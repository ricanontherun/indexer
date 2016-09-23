# Indexer
Trying my hand at an document indexing system. The resulting inverted index will be stored in json format and contain unique tokens mapped to structures containing information such as their documents of origin and line numbers. This might change.

## Usage
```sh
$ indexer /path/to/input/file/or/directory /path/to/output
```
## TODO
* Performance Improvements
    * Actually output the damn file legend
    * Intelligently determine chunk size
    * High performance file I/O with mmap/buffers when it makes sense