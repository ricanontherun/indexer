# Indexer
Trying my hand at an document indexing system. The resulting inverted index will contain unique tokens mapped to structures containing information such as their documents of origin and line numbers. This might change.

At the moment, it works by first splitting each file into a series of 64kb chunks and then dispatching a series of worker threads to index each chunk file.

## (Eventual) Usage
```sh
$ indexer /path/to/file/or/directory --output=/path/to/my/index
```
