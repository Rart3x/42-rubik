import pygame
import tkinter


def handle_events_input(event, rects):
    '''Handle events input method'''

    if event.type == pygame.QUIT:
        pygame.quit()
    elif event.type == pygame.KEYDOWN:
        handle_keys_input(event)
    elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
        mouse_x, mouse_y = pygame.mouse.get_pos()
        for i, rect in rects.items():
            if rect.collidepoint(mouse_x, mouse_y):
                print(f"Button {i} clicked!")


def handle_keys_input(event) -> None:
    '''Handle keys input method'''

    if event.key == pygame.K_f:
        print("F KEY")
    elif event.key == pygame.K_r:
        print("R KEY")
    elif event.key == pygame.K_u:
        print("U KEY")
    elif event.key == pygame.K_b:
        print("B KEY")
    elif event.key == pygame.K_l:
        print("L KEY")
    elif event.key == pygame.K_d:
        print("D KEY")
    elif event.key == pygame.K_ESCAPE:
        pygame.quit()


def display():
    '''Display function for Pygame Rubik's Cube'''

    pygame.init()

    root = tkinter.Tk()
    w, h = root.winfo_screenwidth(), root.winfo_screenheight()

    background_colour = (255, 255, 255)
    button_size = (150, 50)
    button_gap = 10
    total_button_width = sum([button_size[0] + button_gap for _ in range(6)])
    x_offset = (w - total_button_width) / 2

    font = pygame.font.Font(None, 24)
    pygame.display.set_caption('Rubik')
    screen = pygame.display.set_mode((w, h))
    screen.fill(background_colour)

    buttons = []
    rects = {}
    
    for i, letter in enumerate(['F', 'R', 'U', 'B', 'L', 'D']):
        button = pygame.Surface(button_size)
        text = font.render(letter, True, (0, 0, 0))
        rect = text.get_rect(center = (button.get_width() / 2, button.get_height() / 2))
        button.blit(text, rect)
        
        button_rect = pygame.Rect(rect.left, rect.top, rect.width, rect.height)
        rects[i] = button_rect
        buttons.append(button)

    button_y_position = h - sum([button_size[1] + button_gap for _ in range(6)]) // 2

    for i, button in enumerate(buttons):
        x_position = x_offset + i * (button_size[0] + button_gap)
        screen.blit(button, (x_position, button_y_position))
        screen.blit(button, (x_position, button_y_position))

    pygame.display.flip()

    while True:
        for event in pygame.event.get():
            handle_events_input(event, rects)