{ pkgs ? import <nixpkgs> {} }:
let
  libcarbontray = pkgs.callPackage ../libcarbontray/libcarbontray.nix {};
  carbontray = pkgs.callPackage ./carbontray.nix {
    inherit libcarbontray;
  };
in
carbontray
