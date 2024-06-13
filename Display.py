import random
import pygame
import tkinter


def display():
    '''Display function for Pygame Rubik's Cube'''

    pygame.init()

    background_colour = (255, 255, 255)

    font = pygame.font.Font(None, 24)
    pygame.display.set_caption('Rubik')
    screen = pygame.display.set_mode((1200, 800))
    screen.fill(background_colour)

    buttons = []
    rects = {}
    
    display_buttons(buttons, font, ['F', 'R', 'U', 'B', 'L', 'D'], rects, screen, False)
    display_buttons(buttons, font, ['F\'', 'R\'', 'U\'', 'B\'', 'L\'', 'D\''], rects, screen, True)

    pygame.display.flip()

    while True:
        for event in pygame.event.get():
            handle_events_input(event, font, rects, screen)


def display_buttons(buttons, font, letters, rects, screen, sub):

    root = tkinter.Tk()

    w, h = root.winfo_screenwidth(), root.winfo_screenheight()
    w, h = (1200, 800)

    button_gap = 10
    total_button_width = sum([(35, 35)[0] + button_gap for _ in range(6)])
    x_offset = (w - total_button_width) / 2

    button_y_position = h - sum([(35, 35)[1] + button_gap for _ in range(6)]) // 2

    for i, letter in enumerate(letters):
        button = pygame.Surface((35, 35))
        text = font.render(letter, True, (255, 255, 255))
        
        rect = button.get_rect()
        rect.center = (button.get_width() / 2, button.get_height() / 2)
        text_rect = text.get_rect(center=rect.center)
        button.blit(text, text_rect)
        
        rect.x = x_offset + i * ((35, 35)[0] + button_gap)
        
        if sub == False:
            rect.y = button_y_position
        else:
            rect.y = button_y_position+ (35, 35)[1] + button_gap
        
        rects[letter] = rect
        buttons.append(button)

        screen.blit(button, (rect.x, rect.y))


def generate_cube():
    '''Generate random cube method'''

    cube = [[0]*3 for _ in range(3)]
    
    colors = ['red', 'green', 'blue', 'yellow', 'white', 'black']
    for i in range(3):
        for j in range(3):
            cube[i][j] = random.choice(colors)
    
    return cube


def handle_events_input(event, font, rects, screen):
    '''Handle events input method'''

    if event.type == pygame.QUIT:
        pygame.quit()
    elif event.type == pygame.KEYDOWN:
        handle_keys_input(event, font, rects, screen)
    elif event.type == pygame.MOUSEBUTTONDOWN and event.button == 1:
        mouse_x, mouse_y = pygame.mouse.get_pos()
        for i, rect in rects.items():
            if rect.collidepoint(mouse_x, mouse_y):
                print(f"Button {i} clicked!")   


def handle_keys_input(event, font, rects, screen) -> None:
    '''Handle keys input method'''

    if event.key == pygame.K_f:
        button = pygame.Surface((35, 35))
        text = font.render("F", True, (255, 0, 0))
        rects["F"] = text.get_rect(center=(button.get_width() / 2, button.get_height() / 2))
        button.blit(text, rects["F"])
        screen.blit(button, (rects["F"].x, rects["F"].y))
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