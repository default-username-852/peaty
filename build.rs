use std::{env, path::PathBuf};

fn main() {
    let dst = cmake::build(".");

    println!("cargo:rustc-link-search=native={}/build", dst.display());
    println!("cargo:rustc-link-lib=static=peaty");
    println!("cargo:rustc-link-lib=dylib=stdc++");
    println!("cargo:rerun-if-changed=.");

    let bindings = bindgen::Builder::default()
        .clang_args(&["-x", "c++"])
        .allowlist_function("mwc")
        .generate_inline_functions(true) // may be performance concern, there are alternatives
                                         //.opaque_type("std::(?!vector$).*")
        .opaque_type("std::.*")
        .header("./solve_mwc.h")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");

    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("peaty.rs"))
        .expect("Couldn't write bindings!");
}
