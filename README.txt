THE is "The Home Editor"

A far far years ago i was create some Quake1 maps using DOS Quest editor:

http://www.worch.com/wp-content/uploads/2008/07/section-quake-quest.jpg
http://s16.postimg.org/enqn5lg5h/weirdskew2.jpg

It was a shortcut-style editor with single none-splitted view and direct style primitives editing in perspective and orphographic projection. This was looks like nice; and i write my own editor example in same style.

The second idea is to use two matrices instead of single common MODELVIEW from OpenGL: first will translate coordinates from Object to World and second one from World to Camera. Objects and Cameras may be created as a base class childs with common interface to rotate and move it.

View.cpp:333 may help with existing shortcuts: ctrl-clicks will select objects by clicking to it centers, shift-clicks will select vertexes inside selected objects, objects may be moved by dragging any selected vertex.

Alt-arrows rotate, A-Z near/far, also may export primitives to DBaseIV original realization (created just-for-fun at 2003 by me) amd load it back.

All primitives are prizm (Quake "Brushes"): sizes 3x, 4x, etc may be created by pressing 3 or 4.

Thanx for viewing.
