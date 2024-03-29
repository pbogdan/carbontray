{ stdenv
, lib
, pkgs
, meson
, ninja
, nix-gitignore
, pkg-config
, glib
, gtk3
, xorg
}:
stdenv.mkDerivation {
  pname = "carbontray";
  version = "2020-07-11";

  src = nix-gitignore.gitignoreSource [] ./.;

  nativeBuildInputs = [
    meson
    ninja
    pkg-config
  ];

  buildInputs = [
    glib
    gtk3
    xorg.libX11
  ];
}
