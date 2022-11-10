{
  description = "A very basic flake";

  inputs = {
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = { self, nixpkgs, flake-utils }: with flake-utils.lib; eachDefaultSystem (system:
    let
      pkgs = import nixpkgs { inherit system; };
      openmesh = with pkgs; stdenv.mkDerivation rec {
        pname = "openmesh";
        version = "9.0";
        src = builtins.fetchurl {
          url = "https://www.graphics.rwth-aachen.de/media/openmesh_static/Releases/9.0/OpenMesh-9.0.tar.gz";
          sha256 = "sha256:01nhp4nhwc8vs8cr4xgf4v52ifc4b8s7ixcbbbkqqyc22j94qmxr";
        };
        nativeBuildInputs = [ cmake ninja ];
      };
      openflipper = { stdenv, cmake, ninja, qtbase, qwt, qtxmlpatterns, qtx11extras, qtscript }: stdenv.mkDerivation rec {
        pname = "openflipper";
        version = "4.1";
        src = builtins.fetchurl {
          url = "https://www.graphics.rwth-aachen.de/media/openflipper_static/Releases/4.1/OpenFlipper-4.1.tar.gz";
          sha256 = "sha256:1mdk6lxhngpvzv7snx3y6iwv0xq7m919v8psqxvf4clcxnalqvkp";
        };
        dontWrapQtApps = true;
        buildInputs = [ openmesh qtbase qwt qtxmlpatterns qtx11extras qtscript ];
        nativeBuildInputs = [ cmake ninja ];
      };
      ig-projet = { stdenv, lib, cmake, ninja, openflipper, wrapQtAppsHook }: stdenv.mkDerivation {
        pname = "ig-projet";
        version = "master";
        src = ./.;
        buildInputs = [ openmesh openflipper ];
        nativeBuildInputs = [ cmake ninja wrapQtAppsHook ];
      };
    in
    rec {
      packages.openmesh = openmesh;
      packages.openflipper = pkgs.libsForQt5.callPackage openflipper { stdenv = pkgs.clangStdenv; };
      packages.ig-projet = pkgs.libsForQt5.callPackage ig-projet { stdenv = pkgs.clangStdenv; openflipper = packages.openflipper; };
      packages.default = packages.ig-projet;

      apps.ig-projet = mkApp { drv = packages.default; };
      apps.default = apps.ig-projet;
    }
  );
}
