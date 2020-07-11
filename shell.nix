{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  inputsFrom = [
    (import ./default.nix { inherit pkgs; })
  ];

  buildInputs = with pkgs; [
    clang-tools
  ];
}
