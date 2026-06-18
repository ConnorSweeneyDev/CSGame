#include "csb.hpp"

#include <algorithm>
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
    csb::libraries = {
      "kernel32",
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
      csb::target_configuration == RELEASE ? "freetype" : "freetyped",
      csb::target_configuration == RELEASE ? "libpng16" : "libpng16d",
      csb::target_configuration == RELEASE ? "zs" : "zsd",
      csb::target_configuration == RELEASE ? "bz2" : "bz2d",
      "brotlidec",
      "brotlicommon",
      "SDL3_mixer-static",
      "opusfile",
      "opus",
      "ogg",
      "glm",
    };
  else if (csb::host_platform == LINUX)
    csb::libraries = {
      "c",
      "m",
      "pthread",
      "dl",
      "cse",
      "SDL3",
      "SDL3_ttf",
      csb::target_configuration == RELEASE ? "freetype" : "freetyped",
      "png",
      "z",
      csb::target_configuration == RELEASE ? "bz2" : "bz2d",
      "brotlidec",
      "brotlicommon",
      "SDL3_mixer",
      "opusfile",
      "opus",
      "ogg",
      "glm",
    };
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
    csb::format("22.1.5", csb::choose_files({"program/vertex", "program/fragment"}),
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
    csb::choose_files({"program/vertex", "program/fragment"}), {"build/shader/(filename).spv"});

  const auto csp_file =
    csb::path("build") / (csb::target_configuration == RELEASE ? "release" : "debug") / "CSGame.csp";
  struct info
  {
    std::string space{};
    unsigned int width{};
    unsigned int height{};
    unsigned int channels{};
    unsigned int frame_width{};
    unsigned int frame_height{};
    std::vector<csb::aseprite::animation> animations{};
  };
  using data = std::tuple<std::vector<std::byte>, info>;
  csb::embed<data>(
    {"// This file is automatically generated, do not edit manually.\n\n"
     "#pragma once\n\n"
     "#include \"cse/collision.hpp\"\n"
     "#include \"cse/resource.hpp\"\n\n",
     "// This file is automatically generated, do not edit manually.\n\n"
     "#include \"resource.hpp\"\n\n"
     "#include <array>\n"
     "#include <cstdint>\n"
     "#include <utility>\n\n"
     "#include \"cse/collision.hpp\"\n"
     "#include \"cse/csp.hpp\"\n"
     "#include \"cse/numeric.hpp\"\n"
     "#include \"cse/resource.hpp\"\n\n"},
    {nullptr, nullptr, [](const std::filesystem::path &file) -> std::string
     { return file.extension() == ".spv" ? file.stem().stem().string() : file.stem().string(); },
     [](const std::filesystem::path &file) -> data
     {
       info current{};
       std::vector<std::byte> blob{};
       if (file.extension() == ".spv")
         current.space = file.stem().extension() == ".vert" ? "vertex" : "fragment";
       else if (file.extension() == ".ttf")
         current.space = "font";
       else if (file.extension() == ".aseprite")
         current.space = "image";
       else if (file.extension() == ".wav")
         current.space = "sound";
       else if (file.extension() == ".opus")
         current.space = "music";

       if (current.space == "image")
       {
         const auto texture{csb::read_file<csb::aseprite>(file)};
         blob = texture.data;
         current.width = texture.width;
         current.height = texture.height;
         current.channels = texture.channels;
         current.frame_width = texture.resolution.first;
         current.frame_height = texture.resolution.second;
         current.animations = texture.animations;
       }
       else
         blob = csb::read_file<std::vector<std::byte>>(file);
       return data{blob, current};
     },
     nullptr},
    {[](const std::vector<std::tuple<std::filesystem::path, std::string, data>> &files) -> std::string
     {
       std::string result{"namespace csg\n{\n"};
       const auto declare{[&](const std::string &space, const std::string &type)
                          {
                            std::string block{};
                            for (const auto &[file, name, value] : files)
                              if (std::get<1>(value).space == space)
                                block += std::format("    extern cse::{} {};\n", type, name);
                            if (!block.empty()) result += std::format("  namespace {}\n  {{\n{}  }}\n", space, block);
                          }};
       declare("vertex", "vertex");
       declare("fragment", "fragment");
       declare("font", "font");
       declare("image", "image");
       {
         std::string structs{};
         std::string externs{};
         for (const auto &[file, name, value] : files)
         {
           const auto &texture{std::get<1>(value)};
           if (texture.space != "image") continue;
           structs += std::format("      struct {}_animation\n      {{\n", name);
           for (const auto &animation : texture.animations)
             structs += std::format("        const cse::animation {};\n", animation.name);
           structs += "      };\n";
           externs += std::format("    extern const detail::{}_animation {};\n", name, name);
         }
         if (!externs.empty())
           result +=
             "  namespace animation\n  {\n    namespace detail\n    {\n" + structs + "    }\n" + externs + "  }\n";
       }
       {
         const auto hitbox_names{[&](const info &texture)
                                 {
                                   std::vector<std::string> names{};
                                   for (const auto &animation : texture.animations)
                                     for (const auto &frame : animation.hitboxes)
                                       for (const auto &[identifier, bounds] : frame)
                                         if (std::find(names.begin(), names.end(), identifier) == names.end())
                                           names.push_back(identifier);
                                   return names;
                                 }};
         std::string structs{};
         std::string externs{};
         for (const auto &[file, name, value] : files)
         {
           const auto &texture{std::get<1>(value)};
           if (texture.space != "image") continue;
           const auto names{hitbox_names(texture)};
           if (names.empty()) continue;
           structs += std::format("      struct {}_hitbox\n      {{\n", name);
           for (const auto &identifier : names) structs += std::format("        const cse::hitbox {};\n", identifier);
           structs += "      };\n";
           externs += std::format("    extern const detail::{}_hitbox {};\n", name, name);
         }
         if (!externs.empty())
           result += "  namespace hitbox\n  {\n    namespace detail\n    {\n" + structs + "    }\n" + externs + "  }\n";
       }
       declare("sound", "sound");
       declare("music", "music");
       result += "}\n";
       return result;
     },
     [&csp_file](const std::vector<std::tuple<std::filesystem::path, std::string, data>> &files) -> std::string
     {
       csb::csp csp{};
       for (const auto &[file, name, value] : files) csp.append(std::get<0>(value));
       csb::write_file<csb::csp>(csp_file, csp);

       const auto hitbox_names{[&](const info &texture)
                               {
                                 std::vector<std::string> names{};
                                 for (const auto &animation : texture.animations)
                                   for (const auto &frame : animation.hitboxes)
                                     for (const auto &[identifier, bounds] : frame)
                                       if (std::find(names.begin(), names.end(), identifier) == names.end())
                                         names.push_back(identifier);
                                 return names;
                               }};
       std::vector<std::string> blocks{};
       for (const auto &[file, name, value] : files)
       {
         const auto &texture{std::get<1>(value)};
         if (texture.space != "image") continue;
         const unsigned int per_row{texture.width / texture.frame_width};
         const unsigned int per_column{texture.height / texture.frame_height};
         for (const auto &animation : texture.animations)
         {
           const auto start{animation.range.first};
           const auto end{animation.range.second};
           std::size_t index{};
           for (unsigned int frame{start}; frame <= end; ++frame)
           {
             if (index < animation.hitboxes.size() && !animation.hitboxes[index].empty())
             {
               const auto &boxes{animation.hitboxes[index]};
               std::string block{std::format("  static {} std::array<const std::pair<cse::hitbox, cse::rectangle>, {}> "
                                             "{}_texture_{}_frame_{}_hitboxes\n  {{\n    {{",
                                             csb::target_configuration == DEBUG ? "const" : "constexpr", boxes.size(),
                                             name, animation.name, index + 1)};
               std::size_t box{};
               for (const auto &[identifier, bounds] : boxes)
               {
                 block += std::format("{{cse::hitbox(\"{}\"), {{{:#g}, {:#g}, {:#g}, {:#g}}}}}",
                                      name + "." + identifier, bounds[0], bounds[1], bounds[2], bounds[3]);
                 if (box + 1 < boxes.size()) block += ",\n     ";
                 ++box;
               }
               block += "}\n  };";
               blocks.push_back(block);
             }
             ++index;
           }

           std::string block{std::format(
             "  static constexpr std::array<const cse::animation::frame, {}> {}_texture_{}_frames\n  {{\n    {{",
             end - start + 1, name, animation.name)};
           index = 0;
           for (unsigned int frame{start}; frame <= end; ++frame)
           {
             const unsigned int x{frame % per_row};
             const unsigned int y{(per_column - 1) - (frame / per_row)};
             const float top{static_cast<float>((y + 1) * texture.frame_height) / static_cast<float>(texture.height)};
             const float left{static_cast<float>(x * texture.frame_width) / static_cast<float>(texture.width)};
             const float bottom{static_cast<float>(y * texture.frame_height) / static_cast<float>(texture.height)};
             const float right{static_cast<float>((x + 1) * texture.frame_width) / static_cast<float>(texture.width)};
             if (index < animation.hitboxes.size() && !animation.hitboxes[index].empty())
               block += std::format("{{{{{:#g}, {:#g}, {:#g}, {:#g}}}, {:#g}, {}_texture_{}_frame_{}_hitboxes}}", left,
                                    top, right, bottom, animation.times.at(index), name, animation.name, index + 1);
             else
               block += std::format("{{{{{:#g}, {:#g}, {:#g}, {:#g}}}, {:#g}}}", left, top, right, bottom,
                                    animation.times.at(index));
             if (frame < end) block += ",\n     ";
             ++index;
           }
           block += "}\n  };";
           blocks.push_back(block);
         }
       }
       std::string result{"namespace\n{\n"};
       for (std::size_t block{}; block < blocks.size(); ++block)
       {
         result += blocks[block] + "\n";
         if (block + 1 < blocks.size()) result += "\n";
       }
       result += "}\n\n";

       result += "namespace csg\n{\n";
       const auto define{[&](const std::string &space, const std::string &type)
                         {
                           std::string block{};
                           for (const auto &[file, name, value] : files)
                             if (std::get<1>(value).space == space)
                               block += std::format("    cse::{} {}{{}};\n", type, name);
                           if (!block.empty()) result += std::format("  namespace {}\n  {{\n{}  }}\n", space, block);
                         }};
       define("vertex", "vertex");
       define("fragment", "fragment");
       define("font", "font");
       {
         std::string block{};
         for (const auto &[file, name, value] : files)
         {
           const auto &texture{std::get<1>(value)};
           if (texture.space != "image") continue;
           block += "    cse::image " + name + "{{}, " + std::to_string(texture.width) + ", " +
                    std::to_string(texture.height) + ", " + std::to_string(texture.frame_width) + ", " +
                    std::to_string(texture.frame_height) + ", " + std::to_string(texture.channels) + "};\n";
         }
         if (!block.empty()) result += "  namespace image\n  {\n" + block + "  }\n";
       }
       {
         std::string block{};
         for (const auto &[file, name, value] : files)
         {
           const auto &texture{std::get<1>(value)};
           if (texture.space != "image") continue;
           block += std::format("    const detail::{}_animation {}\n    {{\n", name, name);
           for (std::size_t index{}; index < texture.animations.size(); ++index)
           {
             const auto &animation{texture.animations[index]};
             block += std::format("      {{{}_texture_{}_frames, {}, {}}}", name, animation.name, animation.range.first,
                                  animation.range.second);
             block += index + 1 < texture.animations.size() ? ",\n" : "\n";
           }
           block += "    };\n";
         }
         if (!block.empty()) result += "  namespace animation\n  {\n" + block + "  }\n";
       }
       {
         std::string block{};
         for (const auto &[file, name, value] : files)
         {
           const auto &texture{std::get<1>(value)};
           if (texture.space != "image") continue;
           const auto names{hitbox_names(texture)};
           if (names.empty()) continue;
           block += std::format("    const detail::{}_hitbox {}\n    {{\n", name, name);
           for (std::size_t index{}; index < names.size(); ++index)
           {
             block += std::format("      cse::hitbox(\"{}\")", name + "." + names[index]);
             block += index + 1 < names.size() ? ",\n" : "\n";
           }
           block += "    };\n";
         }
         if (!block.empty()) result += "  namespace hitbox\n  {\n" + block + "  }\n";
       }
       define("sound", "sound");
       define("music", "music");
       result += "}\n\n";

       result += "namespace csp\n{\n";
       result += std::format("  constexpr std::uint64_t signature{{{}ull}};\n", csp.signature());
       result += std::format("  const std::array<patch, {}> patches{{{{\n", files.size());
       std::size_t entry{};
       for (const auto &[file, name, value] : files)
       {
         result += std::format("    {{&csg::{}::{}.data, {}, {}}},\n", std::get<1>(value).space, name,
                               csp.table[entry].first, csp.table[entry].second);
         ++entry;
       }
       result += "  }};\n";
       result += "  const manifest instance{\"CSGame.csp\", signature, patches};\n";
       result += "}\n";
       return result;
     }},
    [](const std::filesystem::path &file) -> bool
    {
      return file.extension() == ".spv" || file.extension() == ".ttf" || file.extension() == ".aseprite" ||
             file.extension() == ".wav" || file.extension() == ".opus";
    },
    csb::combine(
      {csb::choose_files({"build/shader"}, [](const auto &file) { return file.stem().extension() == ".vert"; }),
       csb::choose_files({"build/shader"}, [](const auto &file) { return file.stem().extension() == ".frag"; }),
       csb::choose_files({"program/font", "program/texture", "program/sound", "program/music"})}),
    {"program/include/resource.hpp", "program/source/resource.cpp"}, {csp_file});

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
