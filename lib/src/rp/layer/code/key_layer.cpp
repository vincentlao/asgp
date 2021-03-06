/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the rp::help_layer class.
 * \author Julien Jorge
 */
#include "rp/layer/key_layer.hpp"

#include "rp/game_key.hpp"

#include "engine/level.hpp"
#include "engine/level_globals.hpp"
#include "gui/button.hpp"
#include "gui/static_text.hpp"
#include "gui/callback_function.hpp"
#include "input/keyboard.hpp"

#include "rp/rp_gettext.hpp"

#include <boost/bind.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Constructor.
 * \param name The name of the layer.
 */
rp::key_layer::key_layer( const std::string& name )
  : bear::communication::messageable(name), m_visible(false)
{

} // key_layer::key_layer()

/*----------------------------------------------------------------------------*/
/**
 * \brief Initialize the layer.
 */
void rp::key_layer::build()
{
  super::build();

  m_root_window.set_size( get_size() );
  m_root_window.set_background_color( bear::visual::color("#3b3b3b") );
  m_root_window.set_top_left_border_color( bear::visual::color("#a0a0a0") );
  m_root_window.set_bottom_right_border_color( bear::visual::color("#1b1b1b") );

  create_components();

  m_root_window.fit( 10 );
  m_root_window.set_left( ( get_size().x - m_root_window.width() ) / 2 );
  m_root_window.set_bottom( ( get_size().y - m_root_window.height() ) / 2 );

  get_level_globals().register_item(*this);
} // key_layer::build()

/*----------------------------------------------------------------------------*/
/**
 * \brief Inform the layer that a key has been pressed.
 * \param key The value of the pressed key.
 */
bool rp::key_layer::key_pressed( const bear::input::key_info& key )
{
  if ( !m_visible )
    return false;

  bool result = true;

  if ( key.get_code() == bear::input::keyboard::kc_escape )
    hide();
  else if ( key.is_enter() )
    validate();
  else
    result = super::key_pressed(key);

  return result;
} // key_layer::key_pressed()

/*----------------------------------------------------------------------------*/
/**
 * \brief Shows the dialog.
 */
void rp::key_layer::show()
{
  m_visible = true;

  set_root_component( &m_root_window );
  get_level().set_pause();
} //  key_layer::show()

/*----------------------------------------------------------------------------*/
/**
 * \brief Hides the dialog.
 */
void rp::key_layer::hide()
{
  m_visible = false;

  set_root_component( NULL );
  get_level().unset_pause();
} //  key_layer::hide()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the dialog in which the user can enter his code.
 */
void rp::key_layer::create_components()
{
  create_key_text();
  create_label();
  create_validate_button();
  create_cancel_button();
} // key_layer::create_components()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the text area where the user can enter his code. 
 */
void rp::key_layer::create_key_text()
{
  const bear::visual::font f
    ( get_level_globals().get_font( "font/LuckiestGuy.ttf", 20 ) );

  m_key_text =
    new bear::gui::text_input
    ( f, claw::graphic::rgba_pixel( "#fafafa" ) );

  m_key_text->set_border_color( claw::graphic::rgba_pixel( "#a0a0a0") );
  m_key_text->set_width( f.get_metrics( 'w' ).get_advance().x * 37 );
  m_key_text->set_margin( 3, 3 );

  m_key_text->add_enter_callback
    ( bear::gui::callback_function_maker
      ( boost::bind( &key_layer::validate, this ) ) );
  m_key_text->add_changed_callback
    ( bear::gui::callback_function_maker
      ( boost::bind( &key_layer::update_validate_button, this ) ) );

  m_root_window.insert( m_key_text );
} // key_layer::create_key_text()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the label that tells to enter the key. 
 */
void rp::key_layer::create_label()
{
  const bear::visual::font f
    ( get_level_globals().get_font( "font/LuckiestGuy.ttf", 20 ) );

  bear::gui::static_text* const text( new bear::gui::static_text( f ) );
  text->set_text( rp_gettext( "Enter your game key" ) );
  text->set_auto_size( true );

  text->set_bottom( m_key_text->top() + 3 );
  text->set_left( m_key_text->left() );

  m_root_window.insert( text );
} // key_layer::create_label()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the buttons to click to validate the key.
 */
void rp::key_layer::create_validate_button()
{
  m_ok_button =
    new bear::gui::button
    ( get_level_globals().auto_sprite
      ( "gfx/status/buttons-2.png", "check" ) );

  m_ok_button->disable();
  m_ok_button->get_rendering_attributes().set_opacity( 0.5 );
  m_ok_button->add_callback
    ( bear::gui::callback_function_maker
      ( boost::bind( &key_layer::validate, this ) ) );

  m_ok_button->set_margin(2);
  m_ok_button->set_bottom_left
    ( m_key_text->right() + 10, m_key_text->bottom() );
  m_root_window.insert( m_ok_button );
} // key_layer::create_validate_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Creates the button to click to cancel.
 */
void rp::key_layer::create_cancel_button()
{
  bear::gui::button* const button
    ( new bear::gui::button
      ( get_level_globals().auto_sprite
        ( "gfx/status/buttons-2.png", "cross" ) ) );

  button->add_callback
    ( bear::gui::callback_function_maker
      ( boost::bind( &key_layer::hide, this ) ) );

  button->set_margin(2);
  button->set_bottom_left( m_ok_button->right() + 10, m_ok_button->bottom() );
  m_root_window.insert( button );
} // key_layer::create_cancel_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Enables the OK button if and only if the key is valid.
 */
void rp::key_layer::update_validate_button()
{
  if ( game_key::is_valid_key( m_key_text->get_text() ) )
    {
      m_ok_button->enable();
      m_ok_button->get_rendering_attributes().set_opacity( 1 );
    }
  else
    {
      m_ok_button->disable();
      m_ok_button->get_rendering_attributes().set_opacity( 0.5 );
    }
} //  key_layer::update_validate_button()

/*----------------------------------------------------------------------------*/
/**
 * \brief Validates the code and hide the frame if it is valid.
 */
void rp::key_layer::validate()
{
  if ( game_key::is_valid_key( m_key_text->get_text() ) )
    {
      game_key::save( m_key_text->get_text() );
      game_key::check_if_demo_version();
      hide();
    }
} //  key_layer::validate()
