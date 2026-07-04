# Single source of truth for compiler -> stdenv/cc/cxx mapping. Imported by both
# shells.nix and package.nix so `nix build` and `nix develop` share one toolchain
# (no ABI drift). A helper, not a perSystem module -- pulled in via import.
{ pkgs }:
{
  gcc = {
    stdenv = pkgs.gcc16Stdenv;
    cc = "${pkgs.gcc16}/bin/gcc";
    cxx = "${pkgs.gcc16}/bin/g++";
  };
}
