# edl2mkvchapters
Convert EDL file (like Channels DVR emits) to a file suitable to be used
with the `--chapters` option provided by `mkvmerge`

The file created is a full XML-style chapters file, containing chapters
for both the show content and for the commercial breaks. This makes it
easier to load up the mkv file into a video editor later to remove the
commercial breaks.

_Note:_ Some editors (e.g. VideoRedo) only import the 'start' timestamp
of each chapter as a 'chapter mark'.
