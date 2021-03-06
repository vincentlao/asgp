/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.

  See the accompanying license file for details about usage, modification and
  distribution of this file.
*/
/**
 * \file
 * \brief Implementation of the rp::game_key class.
 * \author Julien Jorge
 */
#include "rp/game_key.hpp"

#include "rp/defines.hpp"
#include "rp/game_variables.hpp"

#include "engine/game.hpp"
#include "engine/system/game_filesystem.hpp"
#include "universe/forced_movement/forced_rotation.hpp"

#include <claw/logger.hpp>
#include <fstream>

#include <boost/regex.hpp>

/*----------------------------------------------------------------------------*/
/**
 * \brief Tests if a given string is a valid game key.
 * \param key The key to test.
 */
bool rp::game_key::is_valid_key( std::string key )
{
  const boost::regex expr
    ( "[a-z0-9]{8}(-[a-z0-9]{4}){3}-[a-z0-9]{12}",
      boost::regex::perl | boost::regex::icase );

  return boost::regex_match( key, expr );
} // game_key::is_valid_key()

/*----------------------------------------------------------------------------*/
/**
 * \brief Checks if the game has been bought or not and update the game variable
 *        "demo_version" accordingly.
 */
void rp::game_key::check_if_demo_version()
{
#ifdef RP_DEMO
  game_variables::set_demo_version( RP_DEMO != 0 );
#elif defined( __ANDROID__ )

  game_variables::set_demo_version( false );

#else

  const bear::engine::game& g( bear::engine::game::get_instance() );

  const std::string filename
    ( g.get_game_filesystem().get_custom_config_file_name( RP_KEY_FILE_NAME ) );
 
  std::ifstream f( filename.c_str() );
  std::string key;

  if ( std::getline( f, key ) )
    game_variables::set_demo_version( !is_valid_key( key ) );
  else
    game_variables::set_demo_version( true );

#endif // __ANDROID__

  claw::logger << claw::log_verbose << "Demo mode is "
               << game_variables::is_demo_version() << std::endl;
} // game_key::check_if_demo_version()

/*----------------------------------------------------------------------------*/
/**
 * \brief Saves a given string as the game key.
 * \param key The key to save.
 */
void rp::game_key::save( std::string key )
{
  const bear::engine::game& g( bear::engine::game::get_instance() );

  const std::string filename
    ( g.get_game_filesystem().get_custom_config_file_name( RP_KEY_FILE_NAME ) );
 
  std::ofstream f( filename.c_str() );
  f << key << std::endl;
} // game_key::save()
