Ivann Schlosser

# Description

This mini-package provides a very fast, yet user-friendly way of
exporting large road network data in a graph-like format. It uses the
[**libosmium**](https://osmcode.org/libosmium/index.html) library in the
background and proposes a simple R function to extract the road graph
from a `.osm`, `.osm.pbf` extract file with OSM data.

## Background

While there are already great packages for working with OSM data, I
found myself struggling when the networks were getting big (region,
country scale…). Usually, one would have to use command line tools and
export data to a database before actually being able to use it.
Additionally, for certain use cases, the geometry and most of the tags
asociated to an OSM feature are not essential, and therefore including
them into files ultimately comes at a great cost. So this package solves
one specific use case of building a graph out of OSM road network data
with almost no size limit.

While the graph files produced can be used in any workflow, the intended
use case is with the
[**cppRouting**](https://github.com/vlarmet/cppRouting) package, which
is outstanding for working with large scale road networks right from
your R environment. Additionally, pre-processing of files can done with
the `rosmium` package. For example to select a more specific bounding
box in your data. One condition here is required, is that you export
features in a complete way. Missing nodes can caue problems in the
current version of the package.

# cppRnet

Currently, this package proposes a single function `extract_graph` that
accepts the path to a raw OSM extract file, and writes into the working
directory of the project 2 data sets: nodes.csv, road_segments.csv.

## Output

**nodes.csv** - contains 3 columns: id, lon, lat with respectively the
OSM id of a node, its longitude and latitude.

| id        | lon     | lat     |
|-----------|---------|---------|
| character | numeric | numeric |

**road_segments.csv** - contains at least 3 columns, but can contain
more. The minimum expected columns are from, to, length. The first 2
columns contain node ids.

| from      | to        | length  | highway            |
|-----------|-----------|---------|--------------------|
| character | character | numeric | character (factor) |

In some cases, node ids can be cast to long integers, but this is not a
recommended.

## Setup dependencies

This section is under development, it will be a quick setup guide for
the compiled dependencies.
<!-- While ultimately, this package is easy to use, there are some steps to do in order to set up the dependencies that rely on compiled code. This section covers the minimum setup that should get you going on Mac.  -->

<!-- First, we need to verify that the `C++` compilers are installed and up to date. Start by opening a terminal and typing the following command:  -->
<!-- ``` bash  -->
<!-- which g++-13 -->
<!-- ``` -->
<!-- if this results in an error, then consider installing the package `gcc` from *homebrew*. Once installed, open a new terminal an try again. This time, a path should appear, something like `/opt/homebrew/bin/g++-13`. -->

## Installation

The usage of `cppRnet` relies on the **libosmium** library, which needs
to be installed on the machine. For more details on installation refer
to the official website. On mac, it is recommended to install using
[`brew`](https://brew.sh) with the following terminal command:

``` bash
brew install libosmium
```

Next, install the package itself from github:

``` r
# remotes::install_github('ischlo/cppRnet')
# devtools::install_github('ischlo/cppRnet')
# pak::pak("ischlo/cppRnet")
```

The package will detect the presence of the library and notify you if it
doesn’t see it. Once the library is installed, you will need a OSM
extract file, you can get one by manulally exporting a selected area
from [OSM](openstreetmap.org), or for bigger areas, the best approach is
to download a [geofabrick](https://download.geofabrik.de) extract.

## Example

Once the package and dependencies are installed, the workflow is the
following:

``` r
library(cppRnet)
## basic example code

file <- system.file(package = 'cppRnet','extdata',"map.osm") # path to a local file 

cppRnet::extract_graph(file)

# .... 

nodes <- data.table::fread('nodes.csv')
segments <- data.table::fread('road_segments.csv')

head(nodes)
head(segments)
```

At the end of the execution of this code, you should find the two files
in your working directory (if you don’t know which one is it, run
`getwd()`).

## Notes

This is a early stage development package that could develop more
functionalities for integrating the extremely performant **osmium**
library with a user friendly R workflow. The choice has been made to
first export the data to *.csv* files and only after reading it into R.
This is to avoid potentially creating files that are to big to be
handled by the R environment, while still extracting the data.

## References

There are a few great packages for working with OSM data, they all
provide a wide set of functionalities. If the size of your data is not
excessive, like small city-village scale, `osmdata` might be your tool
of choice in R.
