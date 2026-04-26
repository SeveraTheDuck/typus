{ pkgs, llvm }:
with pkgs;
[
  nixfmt
  cmake
  ninja
  ccache
  cmake-format
  editorconfig-checker
  pre-commit
  doxygen
  graphviz
  llvm.clang-tools
  llvm.lldb
  perl
]
++ pkgs.lib.optionals pkgs.stdenv.isLinux [
  perf
  valgrind
]
