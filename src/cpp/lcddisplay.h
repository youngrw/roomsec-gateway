/* -*- Mode: c++ -*- */
#ifndef _LCDDISPLAY_H_
#define _LCDDISPLAY_H_

#include <boost/shared_ptr.hpp>
#include "ioexpander.h"
#include "display.h"

namespace roomsec {

  class IOExpander;

  /**
   * @brief An LCD Screen attached to a I2C IOExpander.
   * The LCD requires a specific setup to work - there is not many options
   * here.
   */
  class LCDDisplay : public Display {
  public:
    LCDDisplay(boost::shared_ptr<IOExpander> expander);
    ~LCDDisplay();
    void initialize();

    /**
     * @brief Initialize the backlight.
     * Without calling this function you will not be able to control the
     * backlight. All pins must be wired to the same gpio bank. The pins should
     * be configured so that high power will activate the lights.
     *
     * @param gpio The gpio port which has the pins.
     * @param red The pin which controls red.
     * @param green The pin which controls green.
     * @param blue The pin which controls blue.
     */
    void setBacklightPins(IOExpander::GPIO gpio, uint8_t red, uint8_t green, uint8_t blue);

    /**
     * @brief Set the color of the display.
     * This call should only be called after setting the pins which will
     * control the color. You should not combine colors.
     *
     * @param color The color the screen will be.
     */
    void setBacklightColor(Color color);
    
    /**
     * @brief set the cursor to [row, col].
     */
    void setDisplay(int row, int col);

    /**
     * @brief Put the character onto the lcd at the current position
     *
     * @param character
     */
    void putChar(char character);

    void putStr(std::string const& str);

    /**
     * @brief Clear the lcd screen.
     */
    void clear();

    /**
     * @brief Move the cursor to [0,0]
     */
    void home();

    /**
     * Returns the size (rows, cols)
     */
    std::pair<int, int> size();

    /**
     * Returns the height of the display in character rows.
     */
    int rows();

    /**
     * Returns the width of screen in character columns.
     */
    int cols();

  protected:
    /**
     * Strobe the 'enable' pin on the LCD screen.  This will force the LCD to
     * process the current instruction.*/
    void strobe();
    
    /**
     * @brief Send an 8 bit data command.
     * This will send the command in two 4-bit bursts. The RS pin is not
     * touched when using this command.
     *
     * @param data The 8 bit data command.
     */
    void sendDataCmd(uint8_t data);
   
    /**
     * @brief Send an 8 bit command.
     * This sets the RS pin to LOW, making it a command instead of a character.
     *
     * @param data The 8 bit command.
     */
    void putCommand(uint8_t data);

    /**
     * @brief Send a 4 bit command.
     * This will send the lower 4 bits of data to the device.  The RS pin is
     * set to LOW. This command is only usefull when reset'ing the device.
     * @param data The lower 4 bits are the command.
     */
    void put4Command(uint8_t data);

  private:
    IOExpander::GPIO colorGPIO;
    uint8_t color[3];
    boost::shared_ptr<IOExpander> expander;
  };
}

#endif
