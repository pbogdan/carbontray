{
  description = "A very simple standalone tray";

  inputs = {
    flake-utils = {
      url = "github:numtide/flake-utils";
    };

    libcarbontray = {
      url = "github:pbogdan/libcarbontray";
      inputs.nixpkgs.follows = "/nixpkgs";
    };

    nixpkgs = {
      url = "github:NixOS/nixpkgs/nixos-unstable";
    };
  };

  outputs = { self, flake-utils, libcarbontray, nixpkgs }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages = {
          carbontray = pkgs.callPackage ./carbontray.nix {
            libcarbontray = libcarbontray.packages.${system}.libcarbontray;
          };
        };

        defaultPackage = self.packages.${system}.carbontray;
      }
    );
}
