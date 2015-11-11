A far far years ago i was create some Quake1 maps using DOS Quest editor. It was a shortcut-style editor with single none-splitted view and direct style primitives editing in perspective and orphographic projection. This was looks like nice; and i write my own editor example in same style.

The second idea is to use two matrices instead of single common MODELVIEW from OpenGL: first will translate coordinates from Object to World and second one from World to Camera. Objects and Cameras may be created as a base class childs with common interface to rotate and move it.

See README for more details.