### Overview ###

KFS consists of three components:
  1. '''Meta server''': This provides the global namespace for the filesystem.  It keeps the directory structure in-memory in a B-tree.
  1. '''Chunkserver''': Files in KFS are split into _chunks_.  Each chunk is 64MB in size.  Chunks are replicated and striped across chunkservers.
  1. '''Client library''': The client library is linked with applications.  This enables applications to read/write files stored in KFS.

### Meta Server ###

The meta server is the repository for all file meta-data.  It stores the file meta-data such as, directory information, blocks of a file, etc., in-memory in a B-tree.  Operations that  mutate the tree are logged to a log file.  Periodically, via an off-line process, the log files are compacted to create a checkpoint file.  Whenever the metaserver is restarted, it rebuilds the B-tree from the latest checkpoint; it then applies mutations to the tree from the log files to recover system state.

For fault-tolerance, the meta server's logs and checkpoint files should be backed up to a remote node.  The source code contains scripts that use rsync to backup system meta-data.

### Chunkserver ###

Chunkservers store ''chunks'', which are blocks of a file.  Each chunk is 64MB in size.  Chunkserver stores chunks as files in the underlying filesystem.  To protect against data corruptions, Adler-32 checksums are used:
  * On writes, checksums are computed on 64KB block boundaries and saved in the chunk meta-data
  * On reads, checksum verification is done using the saved data

Internally,
  * Each chunk file is named: [file-id].[chunk-id].[version](version.md).
  * Each chunk file has a 16K header, that contains the chunk checksum information.  The checksum information is updated during writes.

When a chunkserver is restarted, it scans the directory containing the chunks to determine the chunks it has.  It then sends that information to the meta server. The meta server validates the blocks and notifies the chunkserver of any _stale_ blocks.  These blocks are those that are not owned by any file in the system.  Whenever a chunkserver receives a stale chunk notification, it deletes those chunks.

### Client library ###

The client library enables applications to access files stored in KFS.  For file operations, the client library interfaces with the meta-data server to get file meta-data:
  * On reads, the client library interfaces with the meta server to determine chunk locations; it then downloads the block from one of the replicas
  * On writes, the client library interfaces with the meta server to determine where to write the chunk; the client library then forwards the data to the first replica; the first replica forwards the data to the next replica and so on.