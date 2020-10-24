import setuptools

setuptools.setup(
    name="stats_server",
    version="1.0.0",
    author="Itay Fintz",
    author_email="itayfi@gmail.com",
    description="A server for seminar 20928",
    url="https://github.com/itayfi/defensive-programming-seminar.git",
    install_requires=[
        "bcrypt==3.2.0",
        "flask",
        "sqlalchemy==1.2.13",
        "pyopenssl"
    ],
    packages=setuptools.find_packages(),
    python_requires=">=3.7",
)
