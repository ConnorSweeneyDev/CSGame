#include "csb.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <format>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

void csb::configure()
{
  csb::target_name = "CSGame";
  csb::target_artifact = EXECUTABLE;
  csb::target_linkage = STATIC;
  csb::target_subsystem = CONSOLE;
  csb::target_configuration = DEBUG;
  csb::cxx_standard = CXX20;
  csb::warning_level = W4;
  csb::include_files = csb::choose_files({"program/include"}, {}, {"program/include/resource.hpp"});
  csb::source_files = csb::choose_files({"program/source"}, {}, {"program/source/resource.cpp"});
  if (csb::host_platform == WINDOWS)
    csb::libraries = {"kernel32",
                      "user32",
                      "shell32",
                      "gdi32",
                      "imm32",
                      "comdlg32",
                      "ole32",
                      "oleaut32",
                      "advapi32",
                      "dinput8",
                      "winmm",
                      "winspool",
                      "setupapi",
                      "uuid",
                      "version",
                      "cse",
                      "SDL3-static",
                      "SDL3_ttf-static",
                      target_configuration == RELEASE ? "freetype" : "freetyped",
                      target_configuration == RELEASE ? "libpng16" : "libpng16d",
                      target_configuration == RELEASE ? "zs" : "zsd",
                      target_configuration == RELEASE ? "bz2" : "bz2d",
                      "brotlidec",
                      "brotlicommon",
                      "SDL3_mixer-static",
                      "opusfile",
                      "opus",
                      "ogg",
                      "glm"};
  else if (csb::host_platform == LINUX)
    csb::libraries = {"c",
                      "m",
                      "pthread",
                      "dl",
                      "cse",
                      "SDL3",
                      "SDL3_ttf",
                      target_configuration == RELEASE ? "freetype" : "freetyped",
                      "png",
                      "z",
                      target_configuration == RELEASE ? "bz2" : "bz2d",
                      "brotlidec",
                      "brotlicommon",
                      "SDL3_mixer",
                      "opusfile",
                      "opus",
                      "ogg",
                      "glm"};
}

int csb::clean()
{
  csb::clean_build();
  csb::clean({"program/include/resource.hpp", "program/source/resource.cpp"});
  return csb::build();
}

int csb::build()
{
  if (!csb::is_subproject)
  {
    csb::generate_clang_format({{"BasedOnStyle", "LLVM"},
                                {"ColumnLimit", "120"},
                                {"IndentWidth", "2"},
                                {"ConstructorInitializerIndentWidth", "2"},
                                {"ContinuationIndentWidth", "2"},
                                {"Language", "Cpp"},
                                {"BreakBeforeBraces", "Allman"},
                                {"AllowShortBlocksOnASingleLine", "true"},
                                {"AllowShortIfStatementsOnASingleLine", "true"},
                                {"AllowShortCaseLabelsOnASingleLine", "true"},
                                {"AllowShortLoopsOnASingleLine", "true"},
                                {"AllowShortFunctionsOnASingleLine", "true"},
                                {"AllowShortLambdasOnASingleLine", "true"},
                                {"AllowShortEnumsOnASingleLine", "true"},
                                {"AllowShortNamespacesOnASingleLine", "true"},
                                {"BreakTemplateDeclarations", "No"},
                                {"IndentPPDirectives", "BeforeHash"},
                                {"IndentCaseLabels", "true"},
                                {"NamespaceIndentation", "All"},
                                {"FixNamespaceComments", "false"}});
    csb::format("22.1.5", csb::choose_files({"program/shader"}),
                {"program/include/resource.hpp", "program/source/resource.cpp"});
  }

  csb::archive_install(
    {csb::host_platform == WINDOWS
       ? "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505.1/dxc_2025_07_14.zip"
       : "https://github.com/microsoft/DirectXShaderCompiler/releases/download/v1.8.2505.1/"
         "linux_dxc_2025_07_14.x86_64.tar.gz",
     "build/dxc",
     {csb::host_platform == WINDOWS ? "bin/" + csb::host_architecture : "bin",
      csb::host_platform == WINDOWS ? "lib/" + csb::host_architecture : "lib"},
     {}});
  if (csb::host_platform == LINUX)
  {
    csb::multi_task_run("chmod +x ()",
                        csb::choose_files({"build/dxc"}, [](const auto &file) { return file.extension() == ""; }),
                        {"build/dxc/executable.(filename)"});
    csb::prepend_environment_variable("LD_LIBRARY_PATH", "build/dxc");
  }
  csb::multi_task_run(
    [](const std::filesystem::path &file, const auto &, const auto &) -> std::string
    {
      return std::format("{} -spirv -T {}_6_0 -E main () -Fo []",
                         csb::host_platform == WINDOWS ? "build\\dxc\\dxc.exe" : "./build/dxc/dxc",
                         file.extension() == ".vert" ? "vs" : "ps");
    },
    csb::choose_files({"program/shader"}), {"build/shader/(filename).spv"});

  using animation_name = std::string;
  using animation_range = std::pair<unsigned int, unsigned int>;
  using animation_times = std::vector<double>;
  using hitbox_name = std::string;
  using hitbox_bounds = std::array<double, 4>;
  using animation_hitboxes = std::vector<std::unordered_map<hitbox_name, hitbox_bounds>>;
  using animations = std::vector<std::tuple<animation_name, animation_range, animation_times, animation_hitboxes>>;
  using frame_dimensions = std::pair<unsigned int, unsigned int>;
  using frame_data = std::pair<frame_dimensions, animations>;
  using binary_data = std::vector<std::byte>;
  using width = unsigned int;
  using height = unsigned int;
  using channels = unsigned int;
  using image_data = std::tuple<width, height, channels>;
  using texture_data = std::tuple<image_data, frame_data>;
  using resource = std::tuple<binary_data, texture_data>;
  csb::embed<resource>(
    {"// This file is automatically generated, do not edit manually.\n\n"
     "#pragma once\n\n"
     "#include \"cse/collision.hpp\"\n"
     "#include \"cse/resource.hpp\"\n\n"
     "namespace csg\n"
     "{\n",
     "// This file is automatically generated, do not edit manually.\n\n"
     "#include \"resource.hpp\"\n\n"
     "#include <array>\n"
     "#include <utility>\n\n"
     "#include \"cse/collision.hpp\"\n"
     "#include \"cse/numeric.hpp\"\n"
     "#include \"cse/resource.hpp\"\n\n"
     "namespace\n"
     "{"},
    {nullptr,
     [](const std::filesystem::path &file, const std::string &name, const resource &) -> std::string
     {
       if (file.extension() == ".spv")
       {
         std::string extension{};
         if (file.stem().extension() == ".vert")
           extension = "vertex";
         else
           extension = "fragment";
         return std::format("\n  static constexpr std::array<const unsigned char, (1)> {}_{}_data{{\n    (0)}};\n",
                            name, extension);
       }
       else if (file.extension() == ".ttf")
         return std::format("\n  static constexpr std::array<const unsigned char, (1)> {}_font_data{{\n    (0)}};\n",
                            name);
       else if (file.extension() == ".wav")
         return std::format("\n  static constexpr std::array<const unsigned char, (1)> {}_sound_data{{\n    (0)}};\n",
                            name);
       else if (file.extension() == ".opus")
         return std::format("\n  static constexpr std::array<const unsigned char, (1)> {}_music_data{{\n    (0)}};\n",
                            name);
       else
         return std::format(
           "\n  static constexpr std::array<const unsigned char, (1)> {}_texture_image{{\n    (0)}};\n(2)\n", name);
     },
     [](const std::filesystem::path &file) -> std::string
     {
       if (file.extension() == ".spv")
         return file.stem().stem().string();
       else
         return file.stem().string();
     },
     [](const std::filesystem::path &file) -> resource
     {
       if (file.extension() == ".spv" || file.extension() == ".ttf" || file.extension() == ".wav" ||
           file.extension() == ".opus")
         return {csb::read_file<binary_data>(file), {}};
       else
       {
         const auto aseprite{csb::read_file<csb::aseprite>(file)};
         animations animations{};
         for (const auto &animation : aseprite.animations)
           animations.emplace_back(animation.name, animation.range, animation.times, animation.hitboxes);
         return {aseprite.data,
                 {{aseprite.width, aseprite.height, aseprite.channels}, {aseprite.resolution, animations}}};
       }
     },
     [](const std::string &name, const resource &data) -> std::vector<std::string>
     {
       std::vector<std::string> results{};
       const auto &[binary_data, texture_data]{data};
       std::string binary_data_result{};
       for (std::size_t index{}; index < binary_data.size(); ++index)
       {
         binary_data_result += csb::byte_to_hex(binary_data[index]);
         if (index < binary_data.size() - 1)
         {
           binary_data_result += ",";
           if ((index + 1) % 16 == 0)
             binary_data_result += "\n    ";
           else
             binary_data_result += " ";
         }
       }
       results.push_back(binary_data_result);
       results.push_back(std::to_string(binary_data.size()));

       const auto &[image_data, frame_data]{texture_data};
       const auto &[width, height, channels]{image_data};
       const auto &[frame_dimensions, animations]{frame_data};
       const auto &[frame_width, frame_height]{frame_dimensions};
       if (width == 0 || height == 0 || channels == 0 || frame_width == 0 || frame_height == 0 || animations.empty())
         return results;
       const unsigned int frames_per_row{width / frame_width};
       const unsigned int frames_per_column{height / frame_height};

       std::string animations_result{};
       for (const auto &animation : animations)
       {
         const auto &[animation_name, animation_range, animation_times, animation_hitboxes]{animation};
         const auto &[start_frame, end_frame]{animation_range};
         std::size_t frame_count{};
         for (unsigned int frame_index{start_frame}; frame_index <= end_frame; ++frame_index)
         {
           if (frame_count < animation_hitboxes.size() && !animation_hitboxes[frame_count].empty())
           {
             const auto &frame_hitboxes{animation_hitboxes[frame_count]};
             animations_result +=
               std::format("  static {} std::array<const std::pair<cse::hitbox, cse::rectangle>, {}> "
                           "{}_texture_{}_frame_{}_hitboxes\n  {{\n    {{",
                           csb::target_configuration == DEBUG ? "const" : "constexpr", frame_hitboxes.size(), name,
                           animation_name, frame_count + 1);
             std::size_t hitbox_index{};
             for (const auto &[hitbox_name, hitbox_bounds] : frame_hitboxes)
             {
               animations_result +=
                 std::format("{{cse::hitbox(\"{}\"), {{{:#g}, {:#g}, {:#g}, {:#g}}}}}", name + "." + hitbox_name,
                             hitbox_bounds[0], hitbox_bounds[1], hitbox_bounds[2], hitbox_bounds[3]);
               if (hitbox_index < frame_hitboxes.size() - 1) animations_result += ",\n     ";
               ++hitbox_index;
             }
             animations_result += "}\n  };\n";
           }
           ++frame_count;
         }

         animations_result += std::format("  static constexpr std::array<const cse::animation::frame, "
                                          "{}> {}_texture_{}_frames\n  {{\n    {{",
                                          end_frame - start_frame + 1, name, animation_name);
         frame_count = 0;
         for (unsigned int frame_index{start_frame}; frame_index <= end_frame; ++frame_index)
         {
           const unsigned int frame_x{frame_index % frames_per_row};
           const unsigned int frame_y{(frames_per_column - 1) - (frame_index / frames_per_row)};
           const float top{static_cast<float>((frame_y + 1) * frame_height) / static_cast<float>(height)};
           const float left{static_cast<float>(frame_x * frame_width) / static_cast<float>(width)};
           const float bottom{static_cast<float>(frame_y * frame_height) / static_cast<float>(height)};
           const float right{static_cast<float>((frame_x + 1) * frame_width) / static_cast<float>(width)};
           bool has_hitboxes{frame_count < animation_hitboxes.size() && !animation_hitboxes[frame_count].empty()};
           if (has_hitboxes)
             animations_result +=
               std::format("{{{{{:#g}, {:#g}, {:#g}, {:#g}}}, {:#g}, {}_texture_{}_frame_{}_hitboxes}}", left, top,
                           right, bottom, animation_times.at(frame_count), name, animation_name, frame_count + 1);
           else
             animations_result += std::format("{{{{{:#g}, {:#g}, {:#g}, {:#g}}}, {:#g}}}", left, top, right, bottom,
                                              animation_times.at(frame_count));
           if (frame_index < end_frame) animations_result += ",\n     ";
           ++frame_count;
         }
         animations_result += "}\n  };";
         if (&animation != &animations.back()) animations_result += "\n";
       }
       results.push_back(animations_result);
       return results;
     }},
    {[](const std::vector<std::tuple<std::filesystem::path, std::string, resource>> &files) -> std::string
     {
       std::string result{};
       const auto is_texture{[](const std::filesystem::path &file)
                             {
                               return file.extension() != ".spv" && file.extension() != ".ttf" &&
                                      file.extension() != ".wav" && file.extension() != ".opus";
                             }};
       const auto group{[&](const std::string &space, const std::string &type, const auto &matches)
                        {
                          std::string block{};
                          for (const auto &[file, name, data] : files)
                            if (matches(file)) block += std::format("    extern const cse::{} {};\n", type, name);
                          if (!block.empty()) result += std::format("  namespace {}\n  {{\n{}  }}\n", space, block);
                        }};
       group("vertex", "vertex", [](const std::filesystem::path &file)
             { return file.extension() == ".spv" && file.stem().extension() == ".vert"; });
       group("fragment", "fragment", [](const std::filesystem::path &file)
             { return file.extension() == ".spv" && file.stem().extension() == ".frag"; });
       group("font", "font", [](const std::filesystem::path &file) { return file.extension() == ".ttf"; });
       group("sound", "sound", [](const std::filesystem::path &file) { return file.extension() == ".wav"; });
       group("music", "music", [](const std::filesystem::path &file) { return file.extension() == ".opus"; });

       std::string images{};
       for (const auto &[file, name, data] : files)
         if (is_texture(file)) images += std::format("    extern const cse::image {};\n", name);
       if (!images.empty()) result += "  namespace image\n  {\n" + images + "  }\n";

       std::string structs{};
       std::string externs{};
       for (const auto &[file, name, data] : files)
       {
         if (!is_texture(file)) continue;
         const auto &animations{std::get<1>(std::get<1>(data)).second};
         structs += std::format("      struct {}_animation\n      {{\n", name);
         for (const auto &[animation_name, animation_range, animation_times, animation_hitboxes] : animations)
           structs += std::format("        const cse::animation {};\n", animation_name);
         structs += "      };\n";
         externs += std::format("    extern const detail::{}_animation {};\n", name, name);
       }
       if (!externs.empty())
         result +=
           "  namespace animation\n  {\n    namespace detail\n    {\n" + structs + "    }\n" + externs + "  }\n";

       std::string hitbox_structs{};
       std::string hitbox_externs{};
       for (const auto &[file, name, data] : files)
       {
         if (!is_texture(file)) continue;
         const auto &animations{std::get<1>(std::get<1>(data)).second};
         std::vector<std::string> hitbox_names{};
         for (const auto &[animation_name, animation_range, animation_times, animation_hitboxes] : animations)
           for (const auto &hitbox : animation_hitboxes)
             for (const auto &[identifier, bounds] : hitbox)
               if (std::find(hitbox_names.begin(), hitbox_names.end(), identifier) == hitbox_names.end())
                 hitbox_names.push_back(identifier);
         if (hitbox_names.empty()) continue;
         hitbox_structs += std::format("      struct {}_hitbox\n      {{\n", name);
         for (const auto &hitbox_name : hitbox_names)
           hitbox_structs += std::format("        const cse::hitbox {};\n", hitbox_name);
         hitbox_structs += "      };\n";
         hitbox_externs += std::format("    extern const detail::{}_hitbox {};\n", name, name);
       }
       if (!hitbox_externs.empty())
         result += "  namespace hitbox\n  {\n    namespace detail\n    {\n" + hitbox_structs + "    }\n" +
                   hitbox_externs + "  }\n";

       return result + "}";
     },
     [](const std::vector<std::tuple<std::filesystem::path, std::string, resource>> &files) -> std::string
     {
       std::string result{"}\n\nnamespace csg\n{\n"};
       const auto is_texture{[](const std::filesystem::path &file)
                             {
                               return file.extension() != ".spv" && file.extension() != ".ttf" &&
                                      file.extension() != ".wav" && file.extension() != ".opus";
                             }};
       const auto group{[&](const std::string &space, const std::string &type, const auto &matches)
                        {
                          std::string block{};
                          for (const auto &[file, name, data] : files)
                            if (matches(file))
                              block += std::format("    const cse::{} {}{{{}_{}_data}};\n", type, name, name, type);
                          if (!block.empty()) result += std::format("  namespace {}\n  {{\n{}  }}\n", space, block);
                        }};
       group("vertex", "vertex", [](const std::filesystem::path &file)
             { return file.extension() == ".spv" && file.stem().extension() == ".vert"; });
       group("fragment", "fragment", [](const std::filesystem::path &file)
             { return file.extension() == ".spv" && file.stem().extension() == ".frag"; });
       group("font", "font", [](const std::filesystem::path &file) { return file.extension() == ".ttf"; });
       group("sound", "sound", [](const std::filesystem::path &file) { return file.extension() == ".wav"; });
       group("music", "music", [](const std::filesystem::path &file) { return file.extension() == ".opus"; });

       std::string images{};
       for (const auto &[file, name, data] : files)
       {
         if (!is_texture(file)) continue;
         const auto &[image_data, frame_data]{std::get<1>(data)};
         const auto &[width, height, channels]{image_data};
         const auto &[frame_width, frame_height]{frame_data.first};
         images += std::format("    const cse::image {}{{{}_texture_image, {}, {}, {}, {}, {}}};\n", name, name, width,
                               height, frame_width, frame_height, channels);
       }
       if (!images.empty()) result += "  namespace image\n  {\n" + images + "  }\n";

       std::string animation_definitions{};
       for (const auto &[file, name, data] : files)
       {
         if (!is_texture(file)) continue;
         const auto &animations{std::get<1>(std::get<1>(data)).second};
         animation_definitions += std::format("    const detail::{}_animation {}\n    {{\n      ", name, name);
         for (const auto &animation : animations)
         {
           const auto &[animation_name, animation_range, animation_times, animation_hitboxes]{animation};
           const auto &[start_frame, end_frame]{animation_range};
           animation_definitions +=
             std::format("{{{}_texture_{}_frames, {}, {}}}", name, animation_name, start_frame, end_frame);
           if (&animation != &animations.back()) animation_definitions += ",\n      ";
         }
         animation_definitions += "\n    };\n";
       }
       if (!animation_definitions.empty()) result += "  namespace animation\n  {\n" + animation_definitions + "  }\n";

       std::string hitbox_definitions{};
       for (const auto &[file, name, data] : files)
       {
         if (!is_texture(file)) continue;
         const auto &animations{std::get<1>(std::get<1>(data)).second};
         std::vector<std::string> hitbox_names{};
         for (const auto &animation : animations)
         {
           const auto &[animation_name, animation_range, animation_times, animation_hitboxes]{animation};
           for (const auto &hitbox : animation_hitboxes)
             for (const auto &[identifier, bounds] : hitbox)
               if (std::find(hitbox_names.begin(), hitbox_names.end(), identifier) == hitbox_names.end())
                 hitbox_names.push_back(identifier);
         }
         if (hitbox_names.empty()) continue;
         hitbox_definitions += std::format("    const detail::{}_hitbox {}\n    {{\n      ", name, name);
         for (std::size_t i{}; i < hitbox_names.size(); ++i)
         {
           hitbox_definitions += std::format("cse::hitbox(\"{}\")", name + "." + hitbox_names[i]);
           if (i < hitbox_names.size() - 1) hitbox_definitions += ",\n      ";
         }
         hitbox_definitions += "\n    };\n";
       }
       if (!hitbox_definitions.empty()) result += "  namespace hitbox\n  {\n" + hitbox_definitions + "  }\n";

       return result + "}";
     }},
    [](const std::filesystem::path &file) -> bool
    {
      return file.extension() == ".spv" || file.extension() == ".ttf" || file.extension() == ".aseprite" ||
             file.extension() == ".wav" || file.extension() == ".opus";
    },
    csb::combine(
      {csb::choose_files({"build/shader"}, [](const auto &file) { return file.stem().extension() == ".vert"; }),
       csb::choose_files({"build/shader"}, [](const auto &file) { return file.stem().extension() == ".frag"; }),
       csb::choose_files({"program/font"}), csb::choose_files({"program/sound"}), csb::choose_files({"program/music"}),
       csb::choose_files({"program/texture"})}),
    {"program/include/resource.hpp", "program/source/resource.cpp"});

  csb::subproject_install({"ConnorSweeneyDev/CSEngine", "0.0.0", COMPILED_LIBRARY});

  csb::generate_clangd({{"Diagnostics", {{"UnusedIncludes", "Strict"}, {"MissingIncludes", "Strict"}}}});
  csb::generate_compile_commands();
  csb::compile();
  csb::link();
  return csb::run();
}

int csb::run()
{
  csb::run_target();
  return csb::success;
}

CSB_MAIN()
