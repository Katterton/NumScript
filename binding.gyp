{
    "targets": [{
        "target_name": "NumScript",
        "cflags!": [ "-fno-exceptions","-fopenmp"],
        "cflags_cc!": [ "-fno-exceptions","-fopenmp"],
          "cflags": ["-gomp"],
        "cflags_cc": ["-gomp"],
        "cflags": ["-fopenmp"],
        "cflags_cc": ["-fopenmp"],
        "sources": [
            "csrc/main.c"

        ],
       
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")"
        ],
        'libraries': [],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}