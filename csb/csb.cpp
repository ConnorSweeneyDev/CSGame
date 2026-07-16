#include "csb.hpp"

#include <string>

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
    csb::libraries = {"kernel32", "user32",   "shell32",  "gdi32",   "imm32",       "comdlg32",
                      "ole32",    "oleaut32", "advapi32", "dinput8", "winmm",       "winspool",
                      "setupapi", "uuid",     "version",  "cse",     "SDL3-static", "SDL3_mixer-static",
                      "opusfile", "opus",     "ogg",      "glm"};
  else if (csb::host_platform == LINUX)
    csb::libraries = {"c", "m", "pthread", "dl", "cse", "SDL3", "SDL3_mixer", "opusfile", "opus", "ogg", "glm"};
}

int csb::clean()
{
  csb::clean_build();
  csb::clean({"program/include/resource.hpp", "program/source/resource.cpp"});
  return csb::build();
}

int csb::build()
{
  csb::subproject_install({"ConnorSweeneyDev/CSEngine", "1.0.0", COMPILED_LIBRARY});

  csb::pack(csb::choose_files({"program/texture"}), csb::choose_files({"program/font"}),
            csb::choose_files({"program/sound"}), csb::choose_files({"program/music"}),
            [](const auto &file) -> std::string
            {
              auto parent{file.parent_path().filename().string()};
              if (parent == "texture" || parent == "font" || parent == "sound" || parent == "music") return "CSGame";
              return parent;
            },
            "csg", {"program/include/resource.hpp", "program/source/resource.cpp"});

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
  csb::format("22.1.8");

  csb::generate_compile_commands();
  csb::generate_clangd({{"Diagnostics", {{"UnusedIncludes", "Strict"}, {"MissingIncludes", "Strict"}}}});

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
