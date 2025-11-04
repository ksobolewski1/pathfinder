import pygame
  

WINDOW_WIDTH = 900
WINDOW_HEIGHT = 900


NODE_RADIUS = 5
NODE_COLOR = (0, 150, 255)  
EDGE_COLOR = (255, 255, 255)
BACKGROUND_COLOR = (30, 30, 30) 


pygame.init()
screen = pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
pygame.display.set_caption("Three Node Example")


class Point():
    def __init__(self, x, y, id):
        self.pos = (x, y)
        self.id = id


triangles = []
with open("out") as out:
    for line in out:
        ids = line.split("-")
        triangles.append([int(id) for id in ids])

pts = []
with open("bw-test") as tst:
    for nr, line in enumerate(tst):
        pos = line.split(" ")
        pts.append(Point(float(pos[0]), float(pos[1]), nr))


running = True
while running:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    screen.fill(BACKGROUND_COLOR)

    for tr in triangles:
        pygame.draw.line(screen, EDGE_COLOR, pts[tr[0]].pos, pts[tr[1]].pos, 2)
        pygame.draw.line(screen, EDGE_COLOR, pts[tr[1]].pos, pts[tr[2]].pos, 2)
        pygame.draw.line(screen, EDGE_COLOR, pts[tr[0]].pos, pts[tr[2]].pos, 2)

    for pt in pts:
        pygame.draw.circle(screen, NODE_COLOR, pt.pos, NODE_RADIUS)

    pygame.display.flip()

pygame.quit()


