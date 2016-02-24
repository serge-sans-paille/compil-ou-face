import ast
code = '''
close = open
fd = close('foo')
'''
tree = ast.parse(code)

class VisitCalls(ast.NodeVisitor):
    def visit_Call(self, node):
        self.generic_visit(node)
        if isinstance(node.func, ast.Name) and node.func.id == 'open':
            print(node.func.id)

VisitCalls().visit(tree)
