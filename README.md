Ivann Schlosser

# Description

This mini-package provides a very fast, yet user-friendly way of
exporting large road network data in a graph-like format. It uses the
[**libosmium**](https://osmcode.org/libosmium/index.html) library in the
background and proposes a simple R function to extract the road graph
from a `.osm`, `.osm.pbf` file.

## Background

While there are already great packages for working with OSM data, I
found myself struggling when the networks were getting big (region,
country scale…).

While the graph files produced can be used in any workflow, the intended
use case is with the
[**cppRouting**](https://github.com/vlarmet/cppRouting) package, which
is outstanding for working with large scale road networks.

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
|           |         |         |

**road_segments.csv** - contains at least 3 columns, but can contain
more. The minimum expected columns are from, to, length.

| from      | to        | length  | highway            |
|-----------|-----------|---------|--------------------|
| character | character | numeric | character (factor) |

In some cases, node ids can be cast to long integers, but this is not a
recommended.

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
```

The package will detect the presence of the library and notify you if it
doesn’t see it. Once the library is installed, you will need a OSM
extract file, you can get one by manulally exporting a selected area
from [OSM](openstreetmap.org), or for bigger areas, the best approach is
to download a [geofabrick](https://download.geofabrik.de) extract.

## Example

Once the package is inst

``` r
library(cppRnet)
## basic example code

file <- 'data/map.osm' # path to a local file 
cppRnet::extract_graph(file)
```

At the end of the execution of this code, you should find the two files
in your working directory (if you don’t know which one is it, run
`getwd()`).
