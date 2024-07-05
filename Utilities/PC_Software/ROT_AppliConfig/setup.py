import setuptools

with open("README.md", 'r') as f:
    long_description = f.read()

setuptools.setup(
    name="AppliCfg",
    version="1.0.1",
    description=("Update application project files"),
    long_description=long_description,
    long_description_content_type="text/markdown",
    packages=["AppliCfg"],
    python_requires='>=3.10',
    install_requires=[
        'click',
    ],
    entry_points={
        "console_scripts": ["AppliCfg=main:main"]
    },
)
