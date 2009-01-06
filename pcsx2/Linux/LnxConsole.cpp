/*  Pcsx2 - Pc Ps2 Emulator
 *  Copyright (C) 2002-2008  Pcsx2 Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "Linux.h"

#define COLOR_RESET		"\033[0m"


// Linux Note : The Linux Console is pretty simple.  It just dumps to the stdio!
// (no console open/close/title stuff tho, so those functions are dummies)
namespace Console
{
	static const char* tbl_color_codes[] = 
	{
		"\033[30m"		// black
	,	"\033[31m"		// red
	,	"\033[32m"		// green
	,	"\033[33m"		// yellow
	,	"\033[34m"		// blue
	,	"\033[35m"		// magenta
	,	"\033[36m"		// cyan
	,	"\033[37m"		// white!
	};

	void SetTitle( const string& title )
	{
	}

	void Open()
	{
	}

	void Close()
	{
	}

	__forceinline bool __fastcall Newline()
	{
		if (Config.PsxOut)
			puts( "\n" );

		if (emuLog != NULL)
		{
			fputs("\n", emuLog);
			fflush( emuLog );
		}

		return false;
	}

	__forceinline bool __fastcall Write( const string& fmt )
	{
		if (Config.PsxOut)
			puts( fmt.c_str() );

		if (emuLog != NULL)
			fputs(fmt.c_str(), emuLog);

		return false;
	}
	
	void __fastcall SetColor( Colors color )
	{
		Msg( tbl_color_codes[color] );
	}

	void ClearColor()
	{
		Msg( COLOR_RESET );
	}
}

namespace Msgbox
{	
	bool Alert( const string& fmt )
	{
		if (!UseGui) 
		{ 
			Error( msg ); 
			return false; 
		}

		GtkWidget *dialog;

		dialog = gtk_message_dialog_new (GTK_WINDOW(MainWindow),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_OK, fmt);
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		
		return false; 
	}

	bool Alert(const string& fmt, VARG_PARAM dummy, ...)
	{
		GtkWidget *dialog;
		string msg;

		va_list list;
		va_start(list, dummy);
		ssprintf(msg, fmt, list);
		va_end(list);

		if (msg[msg.length()-1] == '\n')
			msg[msg.length()-1] = 0;

		if (!UseGui) 
		{ 
			Error( msg ); 
			return false; 
		}
		dialog = gtk_message_dialog_new (GTK_WINDOW(MainWindow),
                                  GTK_DIALOG_DESTROY_WITH_PARENT,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_OK, msg);
		gtk_dialog_run (GTK_DIALOG (dialog));
		gtk_widget_destroy (dialog);
		
		return false; 
	}
}

