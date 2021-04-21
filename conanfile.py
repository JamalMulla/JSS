from conans import ConanFile, tools, CMake
import os
import textwrap

class OpenCVConan(ConanFile):
    """
    This recipe generates OpenCV with a classical setup that is useful for most of the cases.
    If you want to experiment with additional features use the provided options.

    """
    name = 'opencv'
    version = '4.5.2'
    settings = 'os', 'compiler', 'build_type', 'arch'
    description = 'OpenCV recipe for the opencv repository'
    url = 'https://github.com/piponazo/conan-opencv'
    source_url = 'https://github.com/opencv/opencv.git'
    license = 'MIT'
    generators = 'cmake'

    options = {
        'opencv_core': [True, False],
        "fPIC": [True, False],
        "contrib": [True, False],
        "contrib_freetype": [True, False],
        "contrib_sfm": [True, False],
        'opencv_features2d': [True, False], # features2d depends on ml and flann
        'opencv_flann': [True, False],
        'opencv_ml': [True, False],
        'opencv_imgproc': [True, False],
        'opencv_calib3d': [True, False],    # calib3d needs features2d and imgproc
        'opencv_highgui': [True, False],    # high_gui depends on imgcodecs & videoio
        'opencv_imgcodecs': [True, False],
        'opencv_video': [True, False],
        'opencv_videoio': [True, False],
        'opencv_videostab': [True, False],
        'opencv_shape': [True, False],
        'opencv_objdetect': [True, False],
        'opencv_photo': [True, False],
        'opencv_stitching': [True, False],
        'opencv_superres': [True, False],
        'opencv_viz': [True, False],
        'opencv_world': [True, False],      # Generate a single lib with all the modules
        'precompiled_headers': [True, False],
        'ffmpeg': [True, False],
        'webcam': [True, False],
        'gui': ["GTK3", "GTK2", "QT", "WIN", "None"],
        'opencl': [True, False],
        'vtk': [True, False],
        'shared': [True, False],
        'parallel': [False, "tbb", "openmp"],
        "with_jpeg": [False, "libjpeg", "libjpeg-turbo"],
        "with_png": [True, False],
        "with_tiff": [True, False],
        "with_jpeg2000": [False, "jasper", "openjpeg"],
        "with_openexr": [True, False],
        "with_eigen": [True, False],
        "with_webp": [True, False],
        "with_gtk": [True, False],
        "with_quirc": [True, False],
        "with_cuda": [True, False],
        "with_cublas": [True, False]
    }

    default_options = 'opencv_core=True', \
        'fPIC=True', \
        'parallel=tbb', \
        'contrib=False', \
        'contrib_freetype=False', \
        'contrib_sfm=False', \
        'opencv_features2d=False', \
        'opencv_flann=False', \
        'opencv_ml=False', \
        'opencv_imgproc=True', \
        'opencv_calib3d=False', \
        'opencv_highgui=True', \
        'opencv_imgcodecs=True', \
        'opencv_video=True', \
        'opencv_videoio=True', \
        'opencv_videostab=False', \
        'opencv_objdetect=False', \
        'opencv_shape=False', \
        'opencv_photo=False', \
        'opencv_stitching=False', \
        'opencv_superres=False', \
        'opencv_viz=False', \
        'opencv_world=False', \
        'precompiled_headers=True', \
        'ffmpeg=True', \
        'webcam=True', \
        'gui=None', \
        'opencl=False', \
        'vtk=False', \
        'shared=True', \
        'parallel=tbb', \
        'with_jpeg=libjpeg', \
        'with_png=True', \
        'with_tiff=True', \
        'with_jpeg2000=jasper', \
        'with_openexr=True', \
        'with_eigen=True', \
        'with_webp=True', \
        'with_gtk=True', \
        'with_quirc=True', \
        'with_cuda=False', \
        'with_cublas=False'


    short_paths = True
    _cmake = None

    @property
    def _source_subfolder(self):
        return "source_subfolder"

    @property
    def _build_subfolder(self):
        return "build_subfolder"

    @property
    def _contrib_folder(self):
        return "contrib"

    def system_requirements(self):
        if tools.os_info.is_linux:
            installer = tools.SystemPackageTool()
            if tools.os_info.linux_distro == "ubuntu":
                if self.options.gui == "GTK2":
                    installer.install('libgtk2.0-dev')
                elif self.options.gui == "GTK3":
                    installer.install('libgtk-3-dev')

                if self.options.ffmpeg:
                    installer.install('libavcodec-dev libavformat-dev libavutil-dev libswscale-dev libavresample-dev')

                # TODO: When some VTK recipe is available we should use it. Right now we can only use
                # VTK on Linux
                if self.options.vtk:
                    installer.install('libvtk6-dev')

    def _patch_opencv(self):
        #for patch in self.conan_data.get("patches", {}).get(self.version, []):
        #tools.patch(**patch)
        # for directory in ['libjasper', 'libjpeg-turbo', 'libjpeg', 'libpng', 'libtiff', 'libwebp', 'openexr', 'protobuf', 'zlib', 'quirc']:
        #     tools.rmdir(os.path.join(self._source_subfolder, '3rdparty', directory))
        # if self.options.with_openexr:
        #     find_openexr = os.path.join(self._source_subfolder, "cmake", "OpenCVFindOpenEXR.cmake")
        #     tools.replace_in_file(find_openexr,
        #                           r'SET(OPENEXR_ROOT "C:/Deploy" CACHE STRING "Path to the OpenEXR \"Deploy\" folder")',
        #                           "")
        #     tools.replace_in_file(find_openexr, "SET(OPENEXR_LIBSEARCH_SUFFIXES x64/Release x64 x64/Debug)", "")
        #     tools.replace_in_file(find_openexr, "SET(OPENEXR_LIBSEARCH_SUFFIXES Win32/Release Win32 Win32/Debug)", "")
        #
        # tools.replace_in_file(os.path.join(self._source_subfolder, "CMakeLists.txt"), "ANDROID OR NOT UNIX", "FALSE")
        # tools.replace_in_file(os.path.join(self._source_subfolder, "modules", "imgcodecs", "CMakeLists.txt"), "JASPER_", "Jasper_")
        pass

    def configure(self):
        self.requires("zlib/1.2.11")
        if self.options.with_jpeg == "libjpeg":
            self.requires("libjpeg/9d")
        elif self.options.with_jpeg == "libjpeg-turbo":
            self.requires("libjpeg-turbo/2.0.6")
        if self.options.with_jpeg2000 == "jasper":
            self.requires("jasper/2.0.25")
        elif self.options.with_jpeg2000 == "openjpeg":
            self.requires("openjpeg/2.4.0")
        if self.options.with_png:
            self.requires("libpng/1.6.37")
        if self.options.with_openexr:
            self.requires("openexr/2.5.5")
        if self.options.with_tiff:
            self.requires("libtiff/4.2.0")
        if self.options.with_eigen:
            self.requires("eigen/3.3.9")
        if self.options.parallel == "tbb":
            self.requires("tbb/2020.3")
        if self.options.with_webp:
            self.requires("libwebp/1.1.0")
        if self.options.get_safe("contrib_freetype"):
            self.requires("freetype/2.10.4")
            self.requires("harfbuzz/2.7.4")
        if self.options.get_safe("contrib_sfm"):
            self.requires("gflags/2.2.2")
            self.requires("glog/0.4.0")
        if self.options.with_quirc:
            self.requires("quirc/1.1")
        if self.options.get_safe("with_gtk"):
            self.requires("gtk/system")


    def source(self):
        tools.get('https://github.com/opencv/opencv/archive/%s.zip' % self.version)
        os.rename("opencv-{}".format(self.version), self._source_subfolder)

        if (self.options.contrib):
            tools.get('https://github.com/opencv/opencv_contrib/archive/%s.tar.gz' % self.version)
            os.rename("opencv_contrib-{}".format(self.version), self._contrib_folder)

    def _configure_cmake(self):
        if self._cmake:
            return self._cmake
        cmake = CMake(self, parallel=True)
        cmake_args = {
            'BUILD_PACKAGE' : 'OFF',
            'BUILD_PERF_TESTS' : 'OFF',
            'BUILD_TESTS' : 'OFF',
            'BUILD_DOCS' : 'OFF',
            'BUILD_WITH_DEBUG_INFO' : 'OFF',
            'BUILD_EXAMPLES' : 'OFF',

            'BUILD_JPEG' : False,
            'BUILD_PROTOBUF' : 'ON',
            'BUILD_JAVA' : 'OFF',

            'BUILD_CUDA_STUBS' : False,
            'BUILD_FAT_JAVA_LIB' : False,
            'BUILD_IPP_IW' : False,
            'BUILD_ITT' : False,
            'BUILD_JASPER' : False,
            'BUILD_OPENEXR' : False,
            'BUILD_OPENJPEG' : False,
            'BUILD_USE_SYMLINKS' : False,
            'BUILD_opencv_java' : False,
            'BUILD_opencv_java_bindings_gen' : False,
            'BUILD_ZLIB' : True,
            'BUILD_PNG' : False,
            'BUILD_TIFF' : False,
            'BUILD_WEBP' : False,
            'BUILD_TBB' : False,
            'OPENCV_FORCE_3RDPARTY_BUILD' : False,
            'BUILD_opencv_python2' : False,
            'BUILD_opencv_python3' : False,
            'BUILD_opencv_python_bindings_g' : False,
            'BUILD_opencv_python_tests' : False,
            'BUILD_opencv_ts' : False,

            'BUILD_opencv_apps' : 'OFF',
            'BUILD_opencv_calib3d': self.options.opencv_calib3d,
            'BUILD_opencv_core': self.options.opencv_core,
            'BUILD_opencv_features2d': self.options.opencv_features2d,
            'BUILD_opencv_flann': self.options.opencv_flann,
            'BUILD_opencv_highgui': self.options.opencv_highgui,
            'BUILD_opencv_imgcodecs': self.options.opencv_imgcodecs,
            'BUILD_opencv_imgproc': self.options.opencv_imgproc,
            'BUILD_opencv_java_bindings_generator': 'OFF',
            'BUILD_opencv_js': 'OFF',
            'BUILD_opencv_ml': self.options.opencv_ml,
            'BUILD_opencv_objdetect': self.options.opencv_objdetect,
            'BUILD_opencv_photo': self.options.opencv_photo,
            'BUILD_opencv_python_bindings_generator': 'ON',
            'BUILD_opencv_shape': self.options.opencv_shape,
            'BUILD_opencv_stitching': self.options.opencv_stitching,
            'BUILD_opencv_superres' : self.options.opencv_superres,
            'BUILD_opencv_video': self.options.opencv_video,
            'BUILD_opencv_videoio': self.options.opencv_videoio,
            'BUILD_opencv_videostab' : self.options.opencv_videostab,
            'BUILD_opencv_viz' : self.options.opencv_viz,
            'BUILD_opencv_world' : self.options.opencv_world,

            'WITH_FFMPEG' : self.options.ffmpeg,

            'WITH_OPENMP' : self.options.parallel == "openmp",
            'WITH_TBB' : self.options.parallel == "tbb",
            'WITH_CUDA' : self.options.with_cuda,
            'WITH_OPENCL' : self.options.opencl,
            'WITH_GIGEAPI' : 'OFF',
            'WITH_GPHOTO2' : 'OFF',
            'WITH_V4L' : self.options.webcam,
            'WITH_LIBV4L' : self.options.webcam,
            'WITH_MATLAB' : 'OFF',
            'WITH_VTK' : self.options.vtk,
            'WITH_EIGEN' : self.options.with_eigen,

            'WITH_1394' : False,
            'WITH_ADE' : False,
            'WITH_ARAVIS' : False,
            'WITH_CLP' : False,
            'WITH_CUFFT' : False,
            'WITH_NVCUVID' : False,
            'WITH_GSTREAMER' : False,
            'WITH_HALIDE' : False,
            'WITH_HPX' : False,
            'WITH_IMGCODEC_HDR' : False,
            'WITH_IMGCODEC_PFM' : False,
            'WITH_IMGCODEC_PXM' : False,
            'WITH_IMGCODEC_SUNRASTER' : False,
            'WITH_INF_ENGINE' : False,
            'WITH_IPP' : False,
            'WITH_ITT' : False,
            'WITH_LIBREALSENSE' : False,
            'WITH_MFX' : False,
            'WITH_NGRAPH' : False,
            'WITH_OPENCLAMDBLAS' : False,
            'WITH_OPENCLAMDFFT' : False,
            'WITH_OPENCL_SVM' : False,
            'WITH_OPENGL' : False,
            'WITH_OPENNI' : False,
            'WITH_OPENNI2' : False,
            'WITH_OPENVX' : False,
            'WITH_PLAIDML' : False,
            'WITH_PROTOBUF' : False,
            'WITH_PVAPI' : False,
            'WITH_QT' : False,
            'WITH_QUIRC' : False,
            'WITH_VA' : False,
            'WITH_VA_INTEL' : False,
            'WITH_VULKAN' : False,
            'WITH_XIMEA' : False,
            'WITH_XINE' : False,
            'WITH_LAPACK' : False,
            'WITH_GTK' : self.options.get_safe("with_gtk", False),
            'WITH_GTK_2_X' : self.options.get_safe("with_gtk", False),
            'WITH_WEBP' : self.options.with_webp,
            'WITH_JPEG' : self.options.with_jpeg != False,
            'WITH_PNG' : self.options.with_png,
            'WITH_TIFF' : self.options.with_tiff,
            'WITH_JASPER' : self.options.with_jpeg2000 == "jasper",
            'WITH_OPENJPEG' : self.options.with_jpeg2000 == "openjpeg",
            'WITH_OPENEXR' : self.options.with_openexr,
            'HAVE_QUIRC' : self.options.with_quirc , # force usage of quirc requirement
            'WITH_DSHOW' : self.settings.compiler == "Visual Studio",
            'WITH_MSMF' : self.settings.compiler == "Visual Studio",
            'WITH_MSMF_DXVA' : self.settings.compiler == "Visual Studio",
            'OPENCV_MODULES_PUBLIC' : "opencv",

            'ENABLE_PRECOMPILED_HEADERS' : 'OFF',
        }

        if tools.os_info.is_linux:
            if self.options.gui == "GTK2":
                cmake_args.update({'WITH_GTK_2_X': 'ON',
                                   'WITH_GTK': 'ON',
                                   'WITH_QT': 'OFF'
                                   })
            elif self.options.gui == "GTK3":
                cmake_args.update({'WITH_GTK': 'ON',
                                   'WITH_QT': 'OFF'
                                   })
            elif self.options.gui == "QT":
                cmake_args.update({'WITH_QT': 'ON',
                                   'WITH_GTK': 'OFF'
                                   })
            if self.options.with_cuda:
                cmake_args.update({'CUDA_NVCC_FLAGS': '--expt-relaxed-constexpr'})

        elif tools.os_info.is_windows:
            if self.options.gui == "None":
                cmake_args.update({'WITH_WIN32UI': 'OFF'})

        if self.settings.compiler == "Visual Studio":
            cmake_args.update({'BUILD_WITH_STATIC_CRT':
                                   str(self.settings.compiler.runtime).startswith("MT")})

        cmake.configure(source_folder=self._source_subfolder, build_folder=self._build_subfolder, defs=cmake_args)
        self._cmake = cmake
        return cmake


    def build(self):
        # tools.replace_in_file(self._source_subfolder + "/CMakeLists.txt",
        #     "project(OpenCV CXX C)",
        #     """project(OpenCV CXX C)
        #        include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        #        conan_basic_setup()""")
        self._patch_opencv()
        cmake = self._configure_cmake()
        cmake.build()


    def package(self):
        self.copy("LICENSE", dst="licenses", src=self._source_subfolder)
        cmake = self._configure_cmake()
        cmake.install()
        tools.rmdir(os.path.join(self.package_folder, "cmake"))
        if os.path.isfile(os.path.join(self.package_folder, "setup_vars_opencv4.cmd")):
            os.rename(os.path.join(self.package_folder, "setup_vars_opencv4.cmd"),
                      os.path.join(self.package_folder, "res", "setup_vars_opencv4.cmd"))
        self._create_cmake_module_alias_targets(
            os.path.join(self.package_folder, self._module_subfolder, self._module_file),
            {component["target"]:"opencv::{}".format(component["target"]) for component in self._opencv_components}
        )

    @staticmethod
    def _create_cmake_module_alias_targets(module_file, targets):
        content = ""
        for alias, aliased in targets.items():
            content += textwrap.dedent("""\
                if(TARGET {aliased} AND NOT TARGET {alias})
                    add_library({alias} INTERFACE IMPORTED)
                    set_property(TARGET {alias} PROPERTY INTERFACE_LINK_LIBRARIES {aliased})
                endif()
            """.format(alias=alias, aliased=aliased))
        tools.save(module_file, content)

    @property
    def _module_subfolder(self):
        return os.path.join("lib", "cmake")

    @property
    def _module_file(self):
        return "conan-official-{}-targets.cmake".format(self.name)

    @property
    def _opencv_components(self):
        def imageformats_deps():
            components = []
            if self.options.with_jpeg2000:
                components.append("{0}::{0}".format(self.options.with_jpeg2000))
            if self.options.with_png:
                components.append("libpng::libpng")
            if self.options.with_jpeg:
                components.append("{0}::{0}".format(self.options.with_jpeg))
            if self.options.with_tiff:
                components.append("libtiff::libtiff")
            if self.options.with_openexr:
                components.append("openexr::openexr")
            if self.options.with_webp:
                components.append("libwebp::libwebp")
            return components

        def eigen():
            return ["eigen::eigen"] if self.options.with_eigen else []

        def parallel():
            if self.options.parallel:
                return ["tbb::tbb"] if self.options.parallel == "tbb" else ["openmp"]
            return []

        def quirc():
            return ["quirc::quirc"] if self.options.with_quirc else []

        def gtk():
            return ["gtk::gtk"] if self.options.get_safe("with_gtk") else []

        def freetype():
            return ["freetype::freetype"] if self.options.get_safe("contrib_freetype") else []

        def xfeatures2d():
            return ["opencv_xfeatures2d"] if self.options.contrib else []

        opencv_components = [
            {"target": "opencv_core",       "lib": "core",       "requires": ["zlib::zlib"] + parallel() + eigen()},
            {"target": "opencv_flann",      "lib": "flann",      "requires": ["opencv_core"] + eigen()},
            {"target": "opencv_imgproc",    "lib": "imgproc",    "requires": ["opencv_core"] + eigen()},
            {"target": "opencv_ml",         "lib": "ml",         "requires": ["opencv_core"] + eigen()},
            {"target": "opencv_photo",      "lib": "photo",      "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
            {"target": "opencv_features2d", "lib": "features2d", "requires": ["opencv_core", "opencv_flann", "opencv_imgproc"] + eigen()},
            {"target": "opencv_imgcodecs",  "lib": "imgcodecs",  "requires": ["opencv_core", "opencv_imgproc", "zlib::zlib"] + eigen() + imageformats_deps()},
            {"target": "opencv_videoio",    "lib": "videoio",    "requires": ["opencv_core", "opencv_imgproc", "opencv_imgcodecs"] + eigen()},
            {"target": "opencv_calib3d",    "lib": "calib3d",    "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d"]+ eigen()},
            {"target": "opencv_highgui",    "lib": "highgui",    "requires": ["opencv_core", "opencv_imgproc", "opencv_imgcodecs", "opencv_videoio"] + freetype() + eigen() + gtk()},
            {"target": "opencv_objdetect",  "lib": "objdetect",  "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + eigen() + quirc()},
            {"target": "opencv_stitching",  "lib": "stitching",  "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + xfeatures2d() + eigen()},
            {"target": "opencv_video",      "lib": "video",      "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + eigen()},
        ]
        if self.options.contrib:
            opencv_components.extend([
                {"target": "opencv_intensity_transform", "lib": "intensity_transform", "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_phase_unwrapping",    "lib": "phase_unwrapping",    "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_plot",                "lib": "plot",                "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_quality",             "lib": "quality",             "requires": ["opencv_core", "opencv_imgproc", "opencv_ml"] + eigen()},
                {"target": "opencv_reg",                 "lib": "reg",                 "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_surface_matching",    "lib": "surface_matching",    "requires": ["opencv_core", "opencv_flann"] + eigen()},
                {"target": "opencv_xphoto",              "lib": "xphoto",              "requires": ["opencv_core", "opencv_imgproc", "opencv_photo"] + eigen()},
                {"target": "opencv_alphamat",            "lib": "alphamat",            "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_fuzzy",               "lib": "fuzzy",               "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_hfs",                 "lib": "hfs",                 "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_img_hash",            "lib": "img_hash",            "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_line_descriptor",     "lib": "line_descriptor",     "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d"] + eigen()},
                {"target": "opencv_saliency",            "lib": "saliency",            "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d"] + eigen()},
                {"target": "opencv_datasets",            "lib": "datasets",            "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_ml", "opencv_imgcodecs"] + eigen()},
                {"target": "opencv_rapid",               "lib": "rapid",               "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + eigen()},
                {"target": "opencv_rgbd",                "lib": "rgbd",                "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + eigen()},
                {"target": "opencv_shape",               "lib": "shape",               "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d"] + eigen()},
                {"target": "opencv_structured_light",    "lib": "structured_light",    "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_phase_unwrapping", "opencv_features2d", "opencv_calib3d"] + eigen()},
                {"target": "opencv_videostab",           "lib": "videostab",           "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_photo", "opencv_features2d", "opencv_imgcodecs", "opencv_videoio", "opencv_calib3d", "opencv_video"] + eigen()},
                {"target": "opencv_xfeatures2d",         "lib": "xfeatures2d",         "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_ml", "opencv_features2d", "opencv_calib3d", "opencv_shape", ] + eigen()},
                {"target": "opencv_ximgproc",            "lib": "ximgproc",            "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_video"] + eigen()},
                {"target": "opencv_xobjdetect",          "lib": "xobjdetect",          "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_objdetect"] + eigen()},
                {"target": "opencv_aruco",               "lib": "aruco",               "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d"] + eigen()},
                {"target": "opencv_bgsegm",              "lib": "bgsegm",              "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_calib3d", "opencv_video"] + eigen()},
                {"target": "opencv_bioinspired",         "lib": "bioinspired",         "requires": ["opencv_core", "opencv_imgproc", "opencv_imgcodecs", "opencv_videoio", "opencv_highgui"] + eigen()},
                {"target": "opencv_ccalib",              "lib": "ccalib",              "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_videoio", "opencv_calib3d", "opencv_highgui"] + eigen()},
                {"target": "opencv_dpm",                 "lib": "dpm",                 "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_videoio", "opencv_calib3d", "opencv_highgui", "opencv_objdetect"] + eigen()},
                {"target": "opencv_face",                "lib": "face",                "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_photo", "opencv_features2d", "opencv_calib3d", "opencv_objdetect"] + eigen()},
                {"target": "opencv_optflow",             "lib": "optflow",             "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_video", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_video", "opencv_ximgproc"] + eigen()},
                {"target": "opencv_superres",            "lib": "superres",            "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_features2d", "opencv_imgcodecs", "opencv_videoio", "opencv_calib3d", "opencv_video", "opencv_ximgproc", "opencv_optflow"] + eigen()},
                {"target": "opencv_tracking",            "lib": "tracking",            "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_ml", "opencv_plot", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_datasets", "opencv_video"] + eigen()},
                {"target": "opencv_stereo",              "lib": "stereo",              "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_ml", "opencv_plot", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_datasets", "opencv_video", "opencv_tracking"] + eigen()},
            ])

            if self.options.get_safe("contrib_freetype"):
                opencv_components.extend([
                    {"target": "opencv_freetype",   "lib": "freetype",          "requires": ["opencv_core", "opencv_imgproc", "freetype::freetype", "harfbuzz::harfbuzz"] + eigen()},
                ])

            if self.options.get_safe("contrib_sfm"):
                opencv_components.extend([
                    {"target": "opencv_sfm",        "lib": "sfm",               "requires": ["opencv_core", "opencv_flann", "opencv_imgproc", "opencv_ml", "opencv_features2d", "opencv_imgcodecs", "opencv_calib3d", "opencv_shape", "opencv_xfeatures2d", "correspondence", "multiview", "numeric", "glog::glog", "gflags::gflags"] + eigen()},
                    {"target": "numeric",           "lib": "numeric",           "requires": eigen()},
                    {"target": "correspondence",    "lib": "correspondence",    "requires": ["multiview", "glog::glog"] + eigen()},
                    {"target": "multiview",         "lib": "multiview",         "requires": ["numeric", "gflags::gflags"] + eigen()},
                ])


        if self.options.with_cuda:
            opencv_components.extend([
                {"target": "opencv_cudaarithm",     "lib": "cudaarithm",        "requires": ["opencv_core"] + eigen()},
                {"target": "opencv_cudabgsegm",     "lib": "cudabgsegm",        "requires": ["opencv_core", "opencv_video"] + eigen()},
                {"target": "opencv_cudacodec",      "lib": "cudacodec",         "requires": ["opencv_core"] + eigen()},
                {"target": "opencv_features2d",     "lib": "cudafeatures2d",    "requires": ["opencv_core", "opencv_cudafilters"] + eigen()},
                {"target": "opencv_cudafilters",    "lib": "cudafilters",       "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_cudaimgproc",    "lib": "cudaimgproc",       "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_cudalegacy",     "lib": "cudalegacy",        "requires": ["opencv_core", "opencv_video"] + eigen()},
                {"target": "opencv_cudaobjdetect",  "lib": "cudaobjdetect",     "requires": ["opencv_core", "opencv_objdetect"] + eigen()},
                {"target": "opencv_cudaoptflow",    "lib": "cudaoptflow",       "requires": ["opencv_core"] + eigen()},
                {"target": "opencv_cudastereo",     "lib": "cudastereo",        "requires": ["opencv_core", "opencv_calib3d"] + eigen()},
                {"target": "opencv_cudawarping",    "lib": "cudawarping",       "requires": ["opencv_core", "opencv_imgproc"] + eigen()},
                {"target": "opencv_cudev",          "lib": "cudev",             "requires": [] + eigen()},
            ])

        return opencv_components

    def package_info(self):
        version = self.version.split(".")
        version = "".join(version) if self.settings.os == "Windows" else ""
        debug = "d" if self.settings.build_type == "Debug" and self.settings.compiler == "Visual Studio" else ""

        def get_lib_name(module):
            prefix = "" if module in ("correspondence", "multiview", "numeric") else "opencv_"
            return "%s%s%s%s" % (prefix, module, version, debug)

        def add_components(components):
            for component in components:
                conan_component = component["target"]
                cmake_target = component["target"]
                lib_name = get_lib_name(component["lib"])
                requires = component["requires"]
                self.cpp_info.components[conan_component].names["cmake_find_package"] = cmake_target
                self.cpp_info.components[conan_component].names["cmake_find_package_multi"] = cmake_target
                self.cpp_info.components[conan_component].builddirs.append(self._module_subfolder)
                module_rel_path = os.path.join(self._module_subfolder, self._module_file)
                self.cpp_info.components[conan_component].build_modules["cmake_find_package"] = [module_rel_path]
                self.cpp_info.components[conan_component].build_modules["cmake_find_package_multi"] = [module_rel_path]
                self.cpp_info.components[conan_component].libs = [lib_name]
                self.cpp_info.components[conan_component].includedirs.append(os.path.join("include", "opencv4"))
                self.cpp_info.components[conan_component].requires = requires
                if self.settings.os == "Linux":
                    self.cpp_info.components[conan_component].system_libs = ["dl", "m", "pthread", "rt"]

                if self.settings.os == "Android":
                    self.cpp_info.components[conan_component].includedirs = [
                        os.path.join("sdk", "native", "jni", "include")]
                    self.cpp_info.components[conan_component].system_libs.append("log")
                    if int(str(self.settings.os.api_level)) > 20:
                        self.cpp_info.components[conan_component].system_libs.append("mediandk")
                    if not self.options.shared:
                        self.cpp_info.components[conan_component].libdirs.append(
                            os.path.join("sdk", "native", "staticlibs", tools.to_android_abi(str(self.settings.arch))))
                        if conan_component == "opencv_core":
                            self.cpp_info.components[conan_component].libdirs.append("lib")
                            self.cpp_info.components[conan_component].libs += tools.collect_libs(self)

                if self.settings.os == "iOS" or self.settings.os == "Macos":
                    if not self.options.shared:
                        if conan_component == "opencv_core":
                            libs = list(filter(lambda x: not x.startswith("opencv"), tools.collect_libs(self)))
                            self.cpp_info.components[conan_component].libs += libs

                # CMake components names
                cmake_component = component["lib"]
                if cmake_component != cmake_target:
                    conan_component_alias = conan_component + "_alias"
                    self.cpp_info.components[conan_component_alias].names["cmake_find_package"] = cmake_component
                    self.cpp_info.components[conan_component_alias].names["cmake_find_package_multi"] = cmake_component
                    self.cpp_info.components[conan_component_alias].requires = [conan_component]
                    self.cpp_info.components[conan_component_alias].includedirs = []
                    self.cpp_info.components[conan_component_alias].libdirs = []
                    self.cpp_info.components[conan_component_alias].resdirs = []
                    self.cpp_info.components[conan_component_alias].bindirs = []
                    self.cpp_info.components[conan_component_alias].frameworkdirs = []

        self.cpp_info.filenames["cmake_find_package"] = "OpenCV"
        self.cpp_info.filenames["cmake_find_package_multi"] = "OpenCV"

        add_components(self._opencv_components)

        if self.settings.os == "Windows":
            self.cpp_info.components["opencv_highgui"].system_libs = ["comctl32", "gdi32", "ole32", "setupapi", "ws2_32", "vfw32"]
        elif self.settings.os == "Macos":
            self.cpp_info.components["opencv_highgui"].frameworks = ["Cocoa"]
            self.cpp_info.components["opencv_videoio"].frameworks = ["Cocoa", "Accelerate", "AVFoundation", "CoreGraphics", "CoreMedia", "CoreVideo", "QuartzCore"]
        elif self.settings.os == "iOS":
            self.cpp_info.components["opencv_videoio"].frameworks = ["AVFoundation", "QuartzCore"]


    # def package_info(self):
    #     self.cpp_info.includedirs = ['include']  # Ordered list of include paths
    #     for option, activated in self.options.items():
    #         if activated == 'True':
    #             self.cpp_info.libs.append(option)
    #     self.cpp_info.libdirs = ['lib']  # Directories where libraries can be found
    #     self.user_info.shared = self.options.shared
