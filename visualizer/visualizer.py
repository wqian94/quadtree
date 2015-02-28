#!/usr/bin/python3

import re, sys

class Node(object):
    def __init__(self, id, center, length, is_square, parent = None, up = None, down = None, children = {}):
        self.id = id
        self.center = center
        self.length = length
        self.is_square = is_square == 1
        self.parent = parent
        self.up = up
        self.down = down
        self.children = [None, None, None, None]
        for i in children:
            self.children[i] = children[i]
        if id is None or center is None or length is None or is_square is None:
            print("Error in Node construction:")
            print(self)
            input("--- press Enter to continue ---")

    def __repr__(self):
        output = "id = %d, center = %s, length = %f, is_square = %s" % (self.id, self.center, self.length, str(self.is_square))
        if self.parent is not None: output += ", parent = %d" % self.parent
        if self.up is not None: output += ", up = %d" % self.up
        if self.down is not None: output += ", down = %d" % self.down
        if self.children[0] is not None: output += ", children[0] = %d" % self.children[0]
        if self.children[1] is not None: output += ", children[1] = %d" % self.children[1]
        if self.children[2] is not None: output += ", children[2] = %d" % self.children[2]
        if self.children[3] is not None: output += ", children[3] = %d" % self.children[3]

        return output

parse_patterns = {
    "id": re.compile('id=(?P<id>\d+)'),
    "center": re.compile('center=\((?P<center_x>-?(\d*\.\d+|\d+\.?)), (?P<center_y>-?(\d*\.\d+|\d+\.?))\)'),
    "length": re.compile('length=(?P<length>\d+\.\d+)'),
    "is_square": re.compile('is_square=(?P<is_square>[01])'),
    "parent": re.compile('parent=(?P<parent>\d+)'),
    "up": re.compile('up=(?P<up>\d+)'),
    "down": re.compile('down=(?P<down>\d+)'),
    "children_0": re.compile('children\[0\]=(?P<children_0>\d+)'),
    "children_1": re.compile('children\[1\]=(?P<children_1>\d+)'),
    "children_2": re.compile('children\[2\]=(?P<children_2>\d+)'),
    "children_3": re.compile('children\[3\]=(?P<children_3>\d+)'),
}
def parse(line):
    (id, center, length, is_square, parent, up, down, children) = (None, None, None, None, None, None, None, {})
    for p in parse_patterns:
        match = re.search(parse_patterns[p], line)

        if not match:
            continue

        if p == "center":
            value = (match.group(p + "_x"), match.group(p + "_y"))
        else:
            value = match.group(p)

        if value:
            if p == "id":
                id = int(value)
            elif p == "center":
                center = (float(value[0]), float(value[1]))
            elif p == "length":
                length = float(value)
            elif p == "is_square":
                is_square = float(value)
            elif p == "parent":
                parent = float(value)
            elif p == "up":
                up = float(value)
            elif p == "down":
                down = float(value)

            for i in range(0, 4):
                if p == "children_" + str(i):
                    children[i] = float(value)
    return Node(id, center, length, is_square, parent, up, down, children)

def print_instructions():
    print("""
Commands:
=========

% x - repeat line x
clear - clear all history (shortcut: c)
enter x - enters and prints node with id x (shortcut: e x)
help - display this help text (shortcut: h)
history - displays line history (shortcut: y)
find x - displays all ids with matching centers of x, a tuple (error: 1e-6) (shortcut: f x)
ids - display all ids (shortcut: i)
parent - enters the parent of the current node (shortcut: t)
print - prints the current node (shortcut: p)
quit - close this view
""")

tuple_pattern = re.compile('(?P<x>-?(\d*\.\d+|\d+\.?))[^\d.]+(?P<y>-?(\d*\.\d+|\d+\.?))')
def execute(filename):
    print_instructions()

    nodes = {}
    root_id = float("inf")
    with open(filename, "r") as fin:
        for line in fin:
            node = parse(line)
            if node.id is not None:
                nodes[node.id] = node
                root_id = min(root_id, node.id)
            else:
                print("line parse error: %s" % line)
                input("--- press Enter to continue ---")


    line_number = 0
    input_line = input(" %03d> $ " % line_number)
    commands = {}
    current_node = None
    while input_line != "quit":
        commands[line_number] = input_line
        try:
            tokens = input_line.split(" ")

            if type(tokens) == type(""):
                cmd = tokens
                tokens = []
            else:
                cmd = tokens[0]
                tokens = tokens[1:]

            if cmd in ("%",):
                input_line = commands[int(tokens[0])]
                print(input_line)
                continue
            elif cmd in ("clear", "c"):
                commands = {}
            elif cmd in ("enter", "e"):
                current_node = nodes[int(tokens[0])]
                print(current_node)
            elif cmd in ("help", "h"):
                print_instructions()
            elif cmd in ("history", "y"):
                for ln in commands:
                    print("[%03d] %s" % (ln, commands[ln]))
            elif cmd in ("find", "f"):
                match = re.search(tuple_pattern, " ".join(tokens))
                pair = (float(match.group("x")), float(match.group("y")))
                print(pair)
                found_ids = []
                for id in nodes:
                    if abs(nodes[id].center[0] - pair[0]) < 1e-6 and\
                            abs(nodes[id].center[1] - pair[1]) < 1e-6:
                        found_ids += [id]
                print(found_ids)
            elif cmd in ("ids", "i"):
                print(nodes.keys())
            elif cmd in ("print", "p"):
                print(current_node)
            elif cmd in ("parent", "t"):
                current_node = nodes[current_node.parent]
                print(current_node)
            else:
                print("%s is not a valid command. Type `help' for help text." % cmd)
        except:
            print("Error on line %03d: %s" % (line_number, input_line))

        line_number += 1
        input_line = input(" %03d> $ " % line_number)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("USAGE: ./visualizer.py input1 [input2] ...")
        exit(0)

    for filename in sys.argv[1:]:
        execute(filename)
