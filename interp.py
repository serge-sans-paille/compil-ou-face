class Interp(object):
    def __init__(self):
        self.stack = []

    def push(self, val):
        self.stack.append(val)

    def dup(self, _):
        self.stack.append(self.stack[-1])

    def add(self, _):
        arg0 = self.stack.pop()
        arg1 = self.stack.pop()
        self.stack.append(arg0 + arg1)

    def mul(self, _):
        arg0 = self.stack.pop()
        arg1 = self.stack.pop()
        self.stack.append(arg0 * arg1)

    def read(self, _):
        val = int(input('> '))
        self.stack.append(val)

    def write(self, _):
        print(self.stack.pop())

    def eval(self, cmd, arg):
        getattr(self, cmd.lower())(arg)

    def run(self, cmds):
        print(cmds)
        self.peephole(cmds)
        print(cmds)
        for cmd, arg in cmds:
            self.eval(cmd, arg)

    def peephole(self, cmds):
        for i in range(len(cmds) - 1):
            if cmds[i][0] == 'PUSH' and cmds[i][1] == 2 and cmds[i+1][0] == 'MUL':
                cmds[i] = 'DUP', None
                cmds[i+1] = 'ADD', None



interp = Interp()
interp.run([
    ('READ', None),
    ('PUSH', 2),
    ('MUL', None),
    ('WRITE', None),
])
