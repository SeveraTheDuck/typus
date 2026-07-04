# -- Per-compiler dev shells --------------------------------------------------
# Named shells the CI matrix selects via `nix develop .#<compiler>`. They merge
# by name and never touch base's devShells.default. The shell builder lives in
# ./mkCppShell.nix.
# perSystem module.

{ config, pkgs, ... }:
let
  compilers = import ./compilers.nix { inherit pkgs; };
  mkCppShell = import ./mkCppShell.nix { inherit pkgs config; };
in
{
  devShells = {
    gcc = mkCppShell compilers.gcc;
  };
}
