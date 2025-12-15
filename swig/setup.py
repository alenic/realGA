from setuptools import setup, find_packages

setup(
    name="realga",
    version="0.1",
    packages=find_packages(),
    include_package_data=True,
    package_data={"realga": ["_realga.so", "librealga_shared.so"]},
    zip_safe=False,
)