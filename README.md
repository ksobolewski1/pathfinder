# Compile

gcc main.c -o main $(python3-config --includes) $(python3-config --ldflags) -lpython3.12

## Dependencies

- pygame

- sudo apt install python3 python3-dev
