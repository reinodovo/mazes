import base64

def contents(path):
    with open(path, 'r') as file:
        return file.read()
    
template = contents('./manual/template.html')
mazes = contents('./manual/mazes.svg')
mazes = base64.b64encode(mazes.encode('utf-8')).decode('utf-8')
manual = template.replace("{{mazes}}", mazes)
with open('./manual/pages/page1.html', 'w') as file:
    file.write(manual)