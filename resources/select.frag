#version 330 core

uniform uint object_id;

out uint pick;

void main() {
    pick = object_id;
}
