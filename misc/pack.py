import random
from PIL import Image
from typing import List

class Node:
    x = 0
    y = 0
    width = 0
    height = 0
    right = None
    down = None
    used = False
    fit = None

    def __init__(self, x = 0, y = 0, width = 0, height = 0, down = None, right = None, used = False):
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.down = down
        self.right = right
        self.used = used

    def __str__(self):
        return "x: " + str(self.x) + " y: " + str(self.y) + " w: " + str(self.width) + " h: " + str(self.height)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.width == other.width and self.height == other.height

    def __lt__(self, other):
        return max(self.width, self.height) < max(other.width, other.height)


class Packer:
    root : Node = None

    def find_node(self, root : Node, width, height):
        if root.used:
            node = self.find_node(root.right, width, height)
            if not node is None:
                return node
            node = self.find_node(root.down, width, height)
            if not node is None:
                return node
        elif (width <= root.width and height <= root.height):
            return root
        return None

    def grow_node(self, width, height):
        can_grow_down : bool = width <= self.root.width
        can_grow_right : bool = height <= self.root.height

        should_grow_right = can_grow_right and (self.root.height >= (self.root.width + width))
        should_grow_down  = can_grow_down and (self.root.width >= (self.root.height + height))

        if should_grow_right:
            return self.grow_right(width, height)
        elif should_grow_down:
            return self.grow_down(width, height)
        elif can_grow_right:
            return self.grow_right(width, height)
        elif can_grow_down:
            return self.grow_down(width, height)
        else:
            print("Could not grow")
            exit(2)

    def grow_right(self, width, height):
        self.root = Node(0, 0, self.root.width + width, self.root.height, self.root, Node(self.root.width, 0, width, self.root.height), True)
        node = self.find_node(self.root, width, height)
        return self.split_node(node, width, height)

    def grow_down(self, width, height):
        self.root = Node(0, 0, self.root.width, self.root.height + height, Node(0, self.root.height, self.root.width, height), self.root, True)
        node = self.find_node(self.root, width, height)
        return self.split_node(node, width, height)

    def split_node(self, node : Node, width, height):
        node.used = True
        node.down = Node(node.x, node.y + height, node.width, node.height - height)
        node.right = Node(node.x + width, node.y, node.width - width, height)
        return node

    def pack(self, components : List[Node]):
        if len(components) == 0:
            return

        blocks = sorted(components, reverse=True)

        w = blocks[0].width
        h = blocks[0].height

        self.root = Node(x=0, y=0, width=w, height=h)

        for c in components:
            node = self.find_node(self.root, c.width, c.height)
            if not node is None:
                c.fit = self.split_node(node, c.width, c.height)
            else:
                c.fit = self.grow_node(c.width, c.height)
        return self.root



def visualise(components, width, height):
    im= Image.new('RGB', (width, height))

    for com in components:
        c = com.fit
        colour = (random.randint(0, 255),random.randint(0, 255),random.randint(0, 255))
        for row in range(c.y, c.y + com.height, 1):
            for col in range(c.x, c.x + com.width, 1):
                im.putpixel((col, row), colour)

    im.save('packed.png')

components = []

components.append(Node(width=100, height=100))
components.append(Node(width=100, height=100))
components.append(Node(width=80, height=80))
components.append(Node(width=80, height=80))
components.append(Node(width=272, height=32))
components.append(Node(width=76, height=76))
components.append(Node(width=81, height=92))
components.append(Node(width=28, height=2))
components.append(Node(width=92, height=22))
components.append(Node(width=100, height=22))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))
components.append(Node(width=28, height=79))

p = Packer()
root = p.pack(components)


visualise(components, root.width, root.height)


