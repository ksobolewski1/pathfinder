

# LOAD THE DATA FROM A CONFIG FILE
SCREEN_WIDTH = 500
SCREEN_HEIGHT = 500
NODE_SIZE = 250.0
SITE_PACKAGES = '/home/kubaby/venv/lib/python3.11/site-packages'

import sys; 

sys.path.append(SITE_PACKAGES)

import pygame
import pathfinder


def main():

    maze = pathfinder.get_mesh()
    # once the mesh is in, we iterate DEPTH-FIRST SEARCH to reconstruct the order
        # FILL IN ANIMATION
    # then we go again, filling in the maze blocks
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