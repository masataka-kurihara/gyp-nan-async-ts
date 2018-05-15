{
    "targets": [
    {
        "target_name": "helloLib",
        "sources": ["src/helloLib.cc"],
        "include_dirs" : ["<!(node -e \"require('nan')\")"]
    }
  ]
}
