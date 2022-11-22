#version 330 core

uniform uint object_id;

flat in uint vert_id;
out uvec2 pick;

void main() {
    pick = uvec2(object_id, vert_id);
}
