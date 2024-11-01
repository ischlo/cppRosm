
<!-- badges: start -->

[![R-CMD-check](https://github.com/ischlo/cppRosm/actions/workflows/R-CMD-check.yaml/badge.svg)](https://github.com/ischlo/cppRosm/actions/workflows/R-CMD-check.yaml)
![](https://github.com/ischlo/cppSim/actions/workflows/rhub.yaml/badge.svg)
[![Lifecycle:
stable](https://img.shields.io/badge/lifecycle-stable-brightgreen.svg)](https://lifecycle.r-lib.org/articles/stages.html#stable)
![](https://github.com/ischlo/cppRosm/actions/workflows/pkgdown.yaml/badge.svg)

<!-- badges: end -->

## Introduction

This package uses the **libosmium** library in the background and
proposes an opinionated way of interacting with raw OSM data. It is
opinionated, because a number of assumptions are made, based on a long
and sometimes frustrating experience of working with OSM data in R or
python projects. The assumptions and the proposed *data schema* will be
covered in a separate vignette. This part will mainly cover the setup of
the package.

## Setup dependencies

The usage of `cppRosm` relies on the **libosmium** library. The
necessary compiled code dependencies come with the package, facilitating
it’s use.

<!-- This section is under development, it will be a quick setup guide for the compiled dependencies. -->
<!-- While ultimately, this package is easy to use, there are some steps to do in order to set up the dependencies that rely on compiled code. This section covers the minimum setup that should get you going on Mac.  -->
<!-- First, we need to verify that the `C++` compilers are installed and up to date. Start by opening a terminal and typing the following command:  -->
<!-- ``` bash  -->
<!-- which g++-13 -->
<!-- ``` -->
<!-- if this results in an error, then consider installing the package `gcc` from *homebrew*. Once installed, open a new terminal an try again. This time, a path should appear, something like `/opt/homebrew/bin/g++-13`. -->

## Installation

<!-- which needs to be installed on the machine. For more details on installation refer to the official website. On mac, it is recommended to install using [`brew`](https://brew.sh) with the following terminal command:  -->
<!-- ``` bash -->
<!-- brew install libosmium -->
<!-- ``` -->

Install the package from github:

``` r
# remotes::install_github('ischlo/cppRosm')
# devtools::install_github('ischlo/cppRosm')
# pak::pak("ischlo/cppRosm")
```

<!-- The package will detect the presence of the library and notify you if it doesn't see it. -->

Once the library is installed, you will need a OSM extract file, you can
get one by manually exporting a selected area from
[OSM](openstreetmap.org), or for bigger areas, the best approach is to
download a [geofabrick](https://download.geofabrik.de) extract.

## Notes

This is a early stage development package that could develop more
functionalities for integrating the extremely performant **osmium**
library with a user friendly R workflow. The choice has been made for
graph data to first export it to *.csv* files and only after reading it
into R. This is to avoid potentially creating files that are to big to
be handled by the R environment, while still extracting the data.
However, this process can fail sometimes, resulting in loss of unsaved
data in the active R environment. Best practices and the optimal
workflows are covered in a separate vignette.

## References

There are a few great packages for working with OSM data, they all
provide a wide set of functionalities. If the size of your data is not
excessive, like small city/village scale, `osmdata` might be your tool
of choice in R. If the data gets bigger, `osmextract` could be the right
choice. This package aims to be both user friendly and flexible, but
also high performing.

| language | package                                                                                                                                               |
|----------|-------------------------------------------------------------------------------------------------------------------------------------------------------|
| R        | [`osmdata`](https://docs.ropensci.org/osmdata/),[`osmextract`](https://docs.ropensci.org/osmextract/)                                                 |
| python   | [`pydriosm`](https://pypi.org/project/pydriosm/), [`pyrosm`](https://pyrosm.readthedocs.io/en/latest/#), [`osmium`](https://pypi.org/project/osmium/) |
