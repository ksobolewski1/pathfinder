

# LOAD THE DATA FROM A CONFIG FILE
SCREEN_WIDTH = 500
SCREEN_HEIGHT = 500
NODE_SIZE = 50.0
SITE_PACKAGES = '/home/kubaby/venv/lib/python3.11/site-packages'
TIME_DELAY = 10

import sys; 

sys.path.append(SITE_PACKAGES)

import pygame
import pathfinder


pygame.init()
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Maze")


def exit_check():

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            return True
    return False


def path_draw(drawn, path):

    while(True):

        if exit_check():
            break

        pygame.time.delay(TIME_DELAY)
        
        for n in drawn:
            pygame.draw.rect(screen, (0, 255, 0), (n.x, n.y, NODE_SIZE, NODE_SIZE))  
        for n in path:
            pygame.draw.rect(screen, (0, 255, 0), (n.x, n.y, NODE_SIZE, NODE_SIZE))

        pygame.display.flip()


def draw_node_borders(node):

    row_count = SCREEN_HEIGHT // NODE_SIZE
    col_count = SCREEN_WIDTH // NODE_SIZE

    row = node.id // col_count
    col = node.id % col_count
    
    neighbour_ids = [-1, -1, -1, -1]

    if(row > 0): # north
        neighbour_ids[0] = (row - 1) * col_count + col
    if(row < row_count - 1): # south
        neighbour_ids[1] = (row + 1) * col_count + col
    if(col > 0): # west
        neighbour_ids[2] = row * col_count + (col - 1)
    if(col < col_count - 1): # east
        neighbour_ids[3] = row * col_count + (col + 1)

    x = int(node.x)
    y = int(node.y)
    ns = int(NODE_SIZE)
    lw = ns // 10

    lines = [
        ((x, y), (x + ns, y), lw),
        ((x, y + ns), (x + ns, y + ns), lw),
        ((x, y), (x, y + ns), lw),
        ((x + ns, y), (x + ns, y + ns), lw),
    ]

    for index, id in enumerate(neighbour_ids):
        if id == -1:
            continue
        if id not in node.edges:
            pygame.draw.line(screen, (0, 0, 0), lines[index][0], lines[index][1], lines[index][2])


def stack_draw(visited, drawn, stack, nodes):

    while (True):

        if len(stack) > 0:
            node = stack.pop()

        pygame.time.delay(TIME_DELAY)

        if exit_check():
            return False

        screen.fill((0, 0, 0))
            
        for n in visited:
            pygame.draw.rect(screen, (255, 255, 255), (n.x, n.y, NODE_SIZE, NODE_SIZE))  
        for n in drawn:
            pygame.draw.rect(screen, (255, 255, 255), (n.x, n.y, NODE_SIZE, NODE_SIZE))
            draw_node_borders(n)

        pygame.draw.rect(screen, (255, 0, 0), (node.x, node.y, NODE_SIZE, NODE_SIZE))  

        pygame.display.flip()

        if len(stack) > 0:
            drawn.add(node)
            visited.remove(node)
    
    return True


def dfs_draw(visited, stack, nodes, node, start):

    stack.append(node)
    if node not in visited:
        visited.add(node)
    
    if exit_check():
        return False
    
    pygame.time.delay(TIME_DELAY)

    screen.fill((0, 0, 0))
        
    for n in visited:
        pygame.draw.rect(screen, (255, 255, 255), (n.x, n.y, NODE_SIZE, NODE_SIZE))  
        
    pygame.draw.rect(screen, (255, 0, 0), (node.x, node.y, NODE_SIZE, NODE_SIZE))  

    pygame.display.flip()
    
    for neighbor_index in node.edges:
        if nodes[neighbor_index] not in visited:
            dfs_draw(visited, stack, nodes, nodes[neighbor_index], start)


def main():

    START = 0
    maze = pathfinder.get_maze(SCREEN_WIDTH, SCREEN_HEIGHT, START, NODE_SIZE)
    path = pathfinder.get_path(maze, START, 99)

    visited = set()
    drawn = set()
    stack = list()

    dfs_draw(visited, stack, maze.nodes, maze.nodes[START], START)
    stack_draw(visited, drawn, stack, maze.nodes)

    pygame.quit()


if __name__ == "__main__":
    main()