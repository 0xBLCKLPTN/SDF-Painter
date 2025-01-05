import platform
import subprocess
import os
import shutil

def install_opencl():
    os_type = platform.system()

    if os_type == "Windows":
        install_opencl_windows()
    elif os_type == "Darwin":  # macOS
        install_opencl_macos()
    elif os_type == "Linux":
        install_opencl_linux()
    else:
        print(f"Unsupported OS: {os_type}")

def install_opencl_windows():
    print("Installing OpenCL on Windows...")
    # Download and install Intel OpenCL Runtime
    url = "https://github.com/intel/compute-runtime/releases/download/21.35.20773/intel-gmmlib_20.3.0_win_x64.zip"
    subprocess.run(["powershell", "-Command", f"Invoke-WebRequest -Uri {url} -OutFile intel-gmmlib.zip"])
    subprocess.run(["powershell", "-Command", "Expand-Archive -Path intel-gmmlib.zip -DestinationPath ."])
    subprocess.run(["powershell", "-Command", ".\\intel-gmmlib\\setup.exe /S"])
    print("OpenCL installed on Windows.")

def install_opencl_macos():
    print("Installing OpenCL on macOS...")
    # macOS typically comes with OpenCL pre-installed, but you can install additional drivers if needed
    subprocess.run(["brew", "install", "opencl-headers"])
    print("OpenCL installed on macOS.")

def install_opencl_linux():
    print("Installing OpenCL on Linux...")
    # Install OpenCL runtime and drivers
    subprocess.run(["sudo", "apt-get", "update"])
    subprocess.run(["sudo", "apt-get", "install", "-y", "ocl-icd-opencl-dev", "beignet"])
    print("OpenCL installed on Linux.")

def install_glew_glfw3_glm():
    os_type = platform.system()

    if os_type == "Windows":
        install_glew_glfw3_glm_windows()
    elif os_type == "Darwin":  # macOS
        install_glew_glfw3_glm_macos()
    elif os_type == "Linux":
        install_glew_glfw3_glm_linux()
    else:
        print(f"Unsupported OS: {os_type}")

def install_glew_glfw3_glm_windows():
    print("Installing GLEW, GLFW3, and GLM on Windows...")
    # Check if vcpkg is installed
    if not shutil.which("vcpkg"):
        install_vcpkg()
    # Use vcpkg to install GLEW, GLFW3, and GLM
    subprocess.run(["vcpkg", "install", "glew", "glfw3", "glm"])
    print("GLEW, GLFW3, and GLM installed on Windows.")

def install_glew_glfw3_glm_macos():
    print("Installing GLEW, GLFW3, and GLM on macOS...")
    # Use Homebrew to install GLEW, GLFW3, and GLM
    subprocess.run(["brew", "install", "glew", "glfw", "glm"])
    print("GLEW, GLFW3, and GLM installed on macOS.")

def install_glew_glfw3_glm_linux():
    print("Installing GLEW, GLFW3, and GLM on Linux...")
    # Use apt-get to install GLEW, GLFW3, and GLM
    subprocess.run(["sudo", "apt-get", "update"])
    subprocess.run(["sudo", "apt-get", "install", "-y", "libglew-dev", "libglfw3-dev", "libglm-dev"])
    print("GLEW, GLFW3, and GLM installed on Linux.")

def install_vcpkg():
    print("Installing vcpkg...")
    # Clone the vcpkg repository
    subprocess.run(["git", "clone", "https://github.com/microsoft/vcpkg.git"])
    # Bootstrap vcpkg
    subprocess.run([".\\vcpkg\\bootstrap-vcpkg.bat"], shell=True)
    # Add vcpkg to the PATH
    os.environ["PATH"] += os.pathsep + os.path.abspath(".\\vcpkg")
    print("vcpkg installed.")

if __name__ == "__main__":
    install_opencl()
    install_glew_glfw3_glm()
