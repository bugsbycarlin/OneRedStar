#include <string>
#include <array>
#include <vector>
#include <math.h>

#include "all_the_honey.h"

using namespace Honey;

int screen_width;
int screen_height;
bool full_screen;
std::string screen_color;
float sound_volume;
float music_volume;
float fade_in_duration;
float word_duration;
float fade_out_duration;
float fade_out_stagger;
float shake_width;

// star, square, triangle, circle, pentagon, hexagon, octagon, arrow, diamond, heart
std::array<std::string, 10> shapes = {
  "Triangle",
  "Square",
  "Pentagon",
  "Hexagon",
  "Octagon",
  "Circle",
  "Star",
  "Arrow",
  "Diamond",
  "Heart"
};

std::array<std::string, 10> color_names = {"Green", "Red", "Grey", "Blue", "Purple", "Orange", "Yellow", "Black", "Pink", "Brown"};
std::array<std::string, 10> color_values;

std::vector<std::string> sequence_action;
std::vector<float> sequence_timing;


Textbox* number_text;
Textbox* color_text;
Textbox* shape_text;

int number;
std::string shape;
std::string color_name;
std::string color_value;
float size;

bool quit = false;

int randomOutside(int size);
void shuffle(int new_number);
void remake();
void handleInput();
void handleAction();
void render();

int main(int argc, char* args[]) {
  // Load configuration
  if(hot_config->checkAndUpdate() != hot_config->SUCCESS) {
    printf("Failed to load config!\n");
    exit(1);
  }

  screen_width = hot_config->getInt("layout", "screen_width");
  screen_height = hot_config->getInt("layout", "screen_height");
  full_screen = hot_config->getBool("layout", "full_screen");
  screen_color = hot_config->getString("layout", "screen_color");
  sound_volume = hot_config->getFloat("sound", "sound_volume");
  music_volume = hot_config->getFloat("sound", "music_volume");
  fade_in_duration = hot_config->getFloat("animation", "fade_in_duration");
  word_duration = hot_config->getFloat("animation", "word_duration");
  fade_out_duration = hot_config->getFloat("animation", "fade_out_duration");
  fade_out_stagger = hot_config->getFloat("animation", "fade_out_stagger");
  shake_width = hot_config->getFloat("animation", "shake_width");

  // Initialize window, graphics and sound
  Window* window = new Window("Honey Engine", screen_width, screen_height, full_screen);
  graphics->initialize(window);
  sound->initialize();

  // Load assets
  for (int i = 0; i < shapes.size(); i++) {
    graphics->addImage(shapes[i], "Art/" + shapes[i] + ".png");
    sound->addSound(std::to_string(i + 1), "Sound/" + std::to_string(i + 1) + ".wav");
    sound->addSound(shapes[i], "Sound/" + shapes[i] + ".wav");
    sound->addSound(shapes[i] + "s", "Sound/" + shapes[i] + "s.wav");
  }
  for (int i = 0; i < color_names.size(); i++) {
    color_values[i] = hot_config->getString("colors", color_names[i]);
    sound->addSound(color_names[i], "Sound/" + color_names[i] + ".wav");
  }
  sound->addSound("dink_0", "Sound/C_Square2.wav");
  sound->addSound("dink_1", "Sound/C_Square1.wav");

  // Make textboxos with junk default values
  std::string font = hot_config->getString("layout", "font");
  number_text = new Textbox(font, hot_config->getInt("layout", "number_text_font_size"), "a", "#77dd77", 1, 2);
  color_text = new Textbox(font, hot_config->getInt("layout", "color_text_font_size"), "b", "#77dd77", 3, 4);
  shape_text = new Textbox(font, hot_config->getInt("layout", "shape_text_font_size"), "c", "#77dd77", 5, 6);
  
  shuffle(1);

  sound->setSoundVolume(sound_volume);
  sound->setMusicVolume(music_volume);

  // Game loop
  while (!quit) {
    hot_config->checkAndUpdate();

    handleInput();

    handleAction();

    render();
  }

  // Cleanup
  // TO DO destroy all images
  graphics->destroyImage("Star");
  sound->destroyAllMusic();
  sound->destroyAllSounds();
  window->destroy();
}

int randomOutside(int size) {
  int x = logic->randomInt(0, size);
  if (x < (size / 2)) {
    return -x;
  } else {
    return size / 2 + x;
  }
}

void shuffle(int new_number) {
  if (new_number == -1) {
    number = logic->randomInt(1, 11);
  } else {
    number = new_number;
  }
  shape = shapes[logic->randomInt(0, shapes.size())];
  int color_num = logic->randomInt(0, color_values.size());
  color_value = color_values[color_num];
  color_name = color_names[color_num];

  remake();
}

void remake() {
  effects->destroyAllEffects();

  size = 1.0 - number / 10.0;
  if (number > 2) {
    size = 0.8;
  }
  if (number > 4) {
    size = 0.6;
  }

  int margin = hot_config->getInt("layout", "shape_margin_" + std::to_string(number));
  int x = hot_config->getInt("layout", "shape_x_" + std::to_string(number));
  int y = hot_config->getInt("layout", "shape_y_" + std::to_string(number));

  if (number <= 4) {
    layouts->makeTileWrapLayout("shapes", x - margin * (number - 1) / 2.0, y, margin, margin, 4);
  } else if (number % 2 == 0) {
    layouts->makeTileWrapLayout("shapes", x - margin * (number / 2 - 1) / 2.0, y, margin, margin, number / 2);
  } else {
    layouts->makeStaggerWrapLayout("shapes", x - margin * ((number+1)/2 - 1) / 2.0, y, margin, margin, (number + 1) / 2);
  }

  number_text->setText(std::to_string(number));
  color_text->setText(color_name);
  if (number == 1) {
    shape_text->setText(shape);
  } else {
    shape_text->setText(shape + "s");
  }

  number_text->x = (int) (0.5 * (screen_width - number_text->width - color_text->width - shape_text->width - 60));
  number_text->y = hot_config->getInt("layout", "number_text_y");
  color_text->x = number_text->x + number_text->width + 30;
  color_text->y = hot_config->getInt("layout", "color_text_y");
  shape_text->x = number_text->x + number_text->width + color_text->width + 60;
  shape_text->y = hot_config->getInt("layout", "shape_text_y");

  sequence_timing = {};
  sequence_action = {};
  sequence_action.push_back("fade_in");
  sequence_timing.push_back(fade_in_duration);
  sequence_action.push_back("number_word");
  sequence_action.push_back("color_word");
  sequence_action.push_back("shape_word");
  sequence_timing.push_back(word_duration);
  sequence_timing.push_back(word_duration);
  sequence_timing.push_back(word_duration);
  for (int i = 1; i <= number; i++) {
    sequence_action.push_back("fade_out_" + std::to_string(i));
    sequence_timing.push_back(fade_out_stagger);
  }

  float total_animating_duration = fade_in_duration + 3 * word_duration + (number - 1) * fade_out_stagger + fade_out_duration + 0.1;
  logic->makeTimeLock("animating", total_animating_duration);
}

void handleInput() {
  input->processInput();

  if (input->threeQuickKey("escape")) {
    quit = true;
  }

  for (int i = 1; i < 10; i++) {
    if (input->keyDown(std::to_string(i)) && !logic->isTimeLocked("animating")) {
      shuffle(i);
    }
  }
  if (input->keyDown("0") && !logic->isTimeLocked("animating")) {
    shuffle(10);
  }
  std::array<std::string, 9> rando_letters = {"a", "s", "d", "f", "g", "h", "j", "k", "l"};
  for (int i = 0; i < rando_letters.size(); i++) {
    if (input->keyDown(rando_letters[i]) && !logic->isTimeLocked("animating")) {
      shuffle(-1);
    }
  }
  std::array<std::string, 10> color_letters = {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p"};
  for (int i = 0; i < color_letters.size(); i++) {
    if (input->keyDown(color_letters[i]) && !logic->isTimeLocked("animating")) {
      int color_num = i;
      color_value = color_values[color_num];
      color_name = color_names[color_num];

      remake();
    }
  }
  if(input->keyDown("up") && !logic->isTimeLocked("animating")) {
    number = (number + 10) % 10 + 1;
    remake();
  }
  if(input->keyDown("down") && !logic->isTimeLocked("animating")) {
    number = (number + 8) % 10 + 1;
    remake();
  }
}

void handleAction() {
  if (logic->isTimeLocked("animating")) {
    float total = 0;

    for (int i = 0; i < sequence_timing.size(); i++) {
      if (logic->timeSince("animating") >= total && logic->timeSince("animating") <= total + sequence_timing[i]) {
        std::string action = sequence_action[i];
        if (!logic->isTimeLocked(action)) {
          if (action.find("fade_in") != std::string::npos) {
            effects->makeTween("fade_in_fade", 0, 1, fade_in_duration);
            effects->makeShake(action + "_shake", shake_width, fade_in_duration);
            logic->makeTimeLock(action, fade_in_duration);
          } else if (action == "number_word") {
            effects->makeShake(action + "_shake", shake_width, word_duration);
            sound->playSound(std::to_string(number), 1);
            logic->makeTimeLock(action, word_duration);
          } else if (action == "color_word") {
            effects->makeShake(action + "_shake", shake_width, word_duration);
            sound->playSound(color_name, 1);
            logic->makeTimeLock(action, word_duration);
          } else if (action == "shape_word") {
            effects->makeShake(action + "_shake", shake_width, word_duration);
            if (number < 2) {
              sound->playSound(shape, 1);
            } else {
              sound->playSound(shape + "s", 1);
            }
            logic->makeTimeLock(action, word_duration);
          } else if (action.find("fade_out_") != std::string::npos) {
            effects->makeTween(action + "_fade", 1, 0, fade_out_duration);
            effects->makeShake(action + "_shake", shake_width, fade_out_duration);
            logic->makeTimeLock(action, fade_out_duration);
          }
        }
      }
      total += sequence_timing[i];
    }
  }
}

void render() {
  graphics->clearScreen(screen_color);
  graphics->draw2D();

  if (logic->isTimeLocked("animating")) { 
    float time_since = logic->timeSince("animating");
    graphics->setColor(color_value, 1);
    for (int i = 0; i < number; i++) {
      pair p;
      if (number % 2 == 0) {
        p = layouts->tileWrap("shapes", i);
      } else {
        p = layouts->staggerWrap("shapes", i);
      }

      if (effects->check("fade_in_fade")) {
        graphics->setColor(color_value, effects->tween("fade_in_fade", effects->LINEAR));
        p.x += effects->shake("fade_in_shake");
        p.y += effects->shake("fade_in_shake");
      }
      //p.x += effects->shake("fade_out_" + std::to_string(i+1) + "_shake");
      //p.y += effects->shake("fade_out_" + std::to_string(i+1) + "_shake");

      if (effects->check("fade_out_" + std::to_string(i+1) + "_fade")) {
        graphics->setColor(color_value, effects->tween("fade_out_" + std::to_string(i + 1) + "_fade", effects->LINEAR));
      }
      graphics->drawImage(shape, p.x, p.y, true, 0, size);
      graphics->setColor(color_value, 1);
    }

    
    if (effects->check("fade_in_fade")) {
      graphics->setColor("#FFFFFF", effects->tween("fade_in_fade", effects->LINEAR));
    }
    if (effects->check("fade_out_1_fade")) {
      graphics->setColor("#FFFFFF", effects->tween("fade_out_1_fade", effects->LINEAR));
    } 

    number_text->setColor(color_value);
    color_text->setColor(color_value);
    shape_text->setColor(color_value);
    float number_shake_x = effects->shake("number_word_shake");
    float number_shake_y = effects->shake("number_word_shake");
    float color_shake_x = effects->shake("color_word_shake");
    float color_shake_y = effects->shake("color_word_shake");
    float shape_shake_x = effects->shake("shape_word_shake");
    float shape_shake_y = effects->shake("shape_word_shake");

    graphics->translate(number_shake_x, number_shake_y, 0);
    number_text->draw();
    graphics->translate(-number_shake_x, -number_shake_y, 0);

    graphics->translate(color_shake_x, color_shake_y, 0);
    color_text->draw();
    graphics->translate(-color_shake_x, -color_shake_y, 0);

    graphics->translate(shape_shake_x, shape_shake_y, 0);
    shape_text->draw();
    graphics->translate(-shape_shake_x, -shape_shake_y, 0);

  }

  graphics->updateDisplay();
}