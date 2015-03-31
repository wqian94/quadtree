#!/usr/bin/python3

import re, sys

class Node(object):
    def __init__(self, id, center, length, is_square, parent = None, up = None, down = None, children = {}, lock = None):
        self.id = id
        self.center = center
        self.length = length
        self.is_square = is_square == 1
        self.parent = parent
        self.up = up
        self.down = down
        self.children = [None, None, None, None]
        self.lock = lock
        for i in children:
            self.children[i] = children[i]
        if id is None or center is None or length is None or is_square is None:
            print("Error in Node construction:")
            print(self)
            input("--- press Enter to continue ---")

    def __repr__(self):
        output = ""
        if hexmode:
            output += "id = 0x%x" % self.id
        else:
            output += "id = %d" % self.id
        output += ", center = %s, length = %f, is_square = %s" % (self.center, self.length, str(self.is_square))
        if self.parent is not None:
            if hexmode:
                output += ", parent = 0x%x" % self.parent
            else:
                output += ", parent = %d" % self.parent
        if self.up is not None:
            if hexmode:
                output += ", up = 0x%x" % self.up
            else:
                output += ", up = %d" % self.up
        if self.down is not None:
            if hexmode:
                output += ", down = 0x%x" % self.down
            else:
                output += ", down = %d" % self.down
        if self.children[0] is not None:
            if hexmode:
                output += ", children[0] = 0x%x" % self.children[0]
            else:
                output += ", children[0] = %d" % self.children[0]
        if self.children[1] is not None:
            if hexmode:
                output += ", children[1] = 0x%x" % self.children[1]
            else:
                output += ", children[1] = %d" % self.children[1]
        if self.children[2] is not None:
            if hexmode:
                output += ", children[2] = 0x%x" % self.children[2]
            else:
                output += ", children[2] = %d" % self.children[2]
        if self.children[3] is not None:
            if hexmode:
                output += ", children[3] = 0x%x" % self.children[3]
            else:
                output += ", children[3] = %d" % self.children[3]
        if self.lock is not None:
            if hexmode:
                output += ", lock = 0x%x" % self.lock
            else:
                output += ", lock = %d" % self.lock

        return output

parse_patterns = {
    "id": re.compile('id=(?P<id>\d+)'),
    "pointer": re.compile('pointer=(?P<id>0x[a-fA-F0-9]+)'),
    "center": re.compile('center=\((?P<center_x>-?(\d*\.\d+|\d+\.?)), (?P<center_y>-?(\d*\.\d+|\d+\.?))\)'),
    "length": re.compile('length=(?P<length>\d+\.\d+)'),
    "is_square": re.compile('is_square=(?P<is_square>[01])'),
    "parent": re.compile('parent=((?P<parenthex>0x[a-fA-F0-9]+)|(?P<parent>\d+))'),
    "up": re.compile('up=((?P<uphex>0x[a-fA-F0-9]+)|(?P<up>\d+))'),
    "down": re.compile('down=((?P<downhex>0x[a-fA-F0-9]+)|(?P<down>\d+))'),
    "children_0": re.compile('children\[0\]=((?P<children_0hex>0x[a-fA-F0-9]+)|(?P<children_0>\d+))'),
    "children_1": re.compile('children\[1\]=((?P<children_1hex>0x[a-fA-F0-9]+)|(?P<children_1>\d+))'),
    "children_2": re.compile('children\[2\]=((?P<children_2hex>0x[a-fA-F0-9]+)|(?P<children_2>\d+))'),
    "children_3": re.compile('children\[3\]=((?P<children_3hex>0x[a-fA-F0-9]+)|(?P<children_3>\d+))'),
    "lock": re.compile('lock=((?P<lockhex>0x[a-fA-F0-9]+)|(?P<lock>\d+))'),
}
def parse(line):
    (id, center, length, is_square, parent, up, down, children, lock) = (None, None, None, None, None, None, None, {}, None)
    for p in parse_patterns:
        match = re.search(parse_patterns[p], line)

        if not match:
            continue

        if p == "center":
            value = (match.group(p + "_x"), match.group(p + "_y"))
        elif p == "pointer":
            value = match.group("id")
        else:
            try:
                value = match.group(p + "hex")
            except:
                value = match.group(p)

        if value:
            if p == "id":
                id = int(value)
            elif p == "pointer":
                id = int(value, 16)
            elif p == "center":
                center = (float(value[0]), float(value[1]))
            elif p == "length":
                length = float(value)
            elif p == "is_square":
                is_square = float(value)
            elif p == "parent":
                if len(value) > 1 and value[:2] == '0x':
                    parent = int(value, 16)
                else:
                    parent = float(value)
            elif p == "up":
                if len(value) > 1 and value[:2] == '0x':
                    up = int(value, 16)
                else:
                    up = float(value)
            elif p == "down":
                if len(value) > 1 and value[:2] == '0x':
                    down = int(value, 16)
                else:
                    down = float(value)
            elif p == "lock":
                if len(value) > 1 and value[:2] == '0x':
                    lock = int(value, 16)
                else:
                    lock = float(value)

            for i in range(0, 4):
                if p == "children_" + str(i):
                    if len(value) > 1 and value[:2] == '0x':
                        children[i] = int(value, 16)
                    else:
                        children[i] = float(value)
    return Node(id, center, length, is_square, parent, up, down, children, lock)

def print_instructions():
    print("""
Commands:
=========

% x - repeat line x
clear - clear all history (shortcut: c)
enter x - enters and prints node with id x (shortcut: e x)
help - display this help text (shortcut: ?)
history - displays line history (shortcut: y)
find x - displays all ids with matching centers of x, a tuple (error: 1e-6) (shortcut: f x)
hex - display hexadecimal mode (shortcut: h)
hexon - turn hexadecimal mode on (shortcut: h+)
hexoff - turn hexadecimal mode off (shortcut: h-)
ids - display all ids (shortcut: i)
parent - enters the parent of the current node (shortcut: t)
print - prints the current node (shortcut: p)
quit - close this view
""")
    print("Hexadecimal mode is: " + {True: "On", False: "Off"}[hexmode])

tuple_pattern = re.compile('(?P<x>-?(\d*\.\d+|\d+\.?))[^\d.]+(?P<y>-?(\d*\.\d+|\d+\.?))')
hexmode = False
def execute(filename):
    global hexmode
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
        base = {True: 16, False: 10}[hexmode]
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
                input_line = commands[int(tokens[0], base)]
                print(input_line)
                continue
            elif cmd in ("clear", "c"):
                commands = {}
            elif cmd in ("enter", "e"):
                current_node = nodes[int(tokens[0], base)]
                print(current_node)
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
            elif cmd in ("help", "?"):
                print_instructions()
            elif cmd in ("hex", "h"):
                print("Hexademical mode: " + {True: "On", False: "Off"}[hexmode])
            elif cmd in ("hexon", "h+"):
                hexmode = True
                print("Hexadecimal mode: On")
            elif cmd in ("hexoff", "h-"):
                hexmode = False
                print("Hexadecimal mode: Off")
            elif cmd in ("history", "y"):
                for ln in commands:
                    print("[%03d] %s" % (ln, commands[ln]))
            elif cmd in ("ids", "i"):
                if hexmode:
                    output = "["
                    for k in nodes.keys():
                        output += "0x%x , " % k
                    output += "]"
                    print(output)
                else:
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
