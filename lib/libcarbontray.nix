{ stdenv
, lib
, meson
, ninja
, nix-gitignore
, pkg-config
, gtk3
, xorg
}:
stdenv.mkDerivation {
  pname = "libcarbontray";
  version = "2021-09-20";

  src = nix-gitignore.gitignoreSource [] ./.;

  nativeBuildInputs = [
    meson
    ninja
    pkg-config
  ];

  buildInputs = [
    gtk3
    xorg.libX11
    xorg.libXcomposite
  ];
}
