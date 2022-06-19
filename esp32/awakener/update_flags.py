Import("env")

with open(".env", "r") as f:
    for line in f.readlines():
        name, value = line.strip().split("=")
        env.Append(BUILD_FLAGS=[f'-D {name}=\\"{value}\\"'])