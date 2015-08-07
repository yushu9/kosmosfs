### Using KFS as the filesystem for Map/Reduce jobs ###

KFS is integrated with Hadoop so that it can be used the backing store for Map/Reduce jobs.  This integration is done using the filesystem APIs provided by Hadoop.  In Hadoop's conf directory (such as, hadoop/conf), edit site.xml:

```
<property>
  <name>fs.kfs.metaServerHost</name>
  <value><server></value>
  <description>The location of the meta server.</description>
</property>

<property>
  <name>fs.kfs.metaServerPort</name>
  <value><port></value>
  <description>The location of the meta server's port.</description>
</property>
```

This enables path URI's of the form
```
kfs://host1:20000/foo
```
to be sent to KFS.

If you want KFS as the default filesystem with Hadoop, update site.xml:
```
<property>
  <name>fs.default.name</name>
  <value>kfs://<server:port></value>
</property>
```

The Hadoop distribution contains a KFS jar file.  To use the latest files, build the jar file (see HowToCompile).  The remaining steps are:
  1. Copy the jar file
  1. Update the LD\_LIBRARY\_PATH environment variable so that libkfsClient.so can be loaded
  1. Start the Map/Reduce job trackers.
```
cp ~/code/kfs/build/kfs-0.5.jar hadoop/lib
cp ~/code/kfs/build/lib/*.so hadoop/lib/native/Linux-amd64-64
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/code/kfs/build/lib
hadoop/bin/start-mapred.sh
```

The Hadoop utilities can be used to view the KFS directory.  For example,
```
bin/hadoop fs -fs kfs://<server:port> -ls /
```
can be used to list the contents of the root directory.

### Using KFS with Hadoop 0.18x, 0.19.0 ###

A new API was introduced in 0.18x to the filesystem layer in Hadoop.  This caused a  change  to be made to src/core/org/apache/fs/kfs/KosmosFileSystem.java.  That change introduced a bug (see hadoop JIRA-4697 and hadoop JIRA-5292).  Fixes for these bugs have been submitted for inclusion to Hadoop project.  If you are using 0.18x, 0.19, 0.19.1 of Hadoop, you will need to include these patches and recompile Hadoop+KFS.