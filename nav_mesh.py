
# LOAD THE DATA FROM A CONFIG FILE
SCREEN_WIDTH = 500
SCREEN_HEIGHT = 500
NODE_SIZE = 250.0
SITE_PACKAGES = '/home/ksobolewski/venv/lib/python3.12/site-packages'

import sys; 

sys.path.append(SITE_PACKAGES)

import pygame
import pathfinder


def main():

    nav_mesh = pathfinder.get_mesh()
    # draw the mesh step by step
    # start the node-placing and selecting program
    path = pathfinder.get_path()
    # display the path step by step

    pygame.init()
    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
    pygame.display.set_caption("Maze")

    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        pygame.display.flip()

    # Quit Pygame
    pygame.quit()


if __name__ == "__main__":
    main()