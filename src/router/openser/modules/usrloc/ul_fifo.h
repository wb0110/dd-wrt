/*
 * $Id: ul_fifo.h,v 1.1.1.1 2005/06/13 16:47:48 bogdan_iancu Exp $
 *
 * Copyright (C) 2001-2003 FhG Fokus
 *
 * This file is part of openser, a free SIP server.
 *
 * openser is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version
 *
 * openser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef _UL_FIFO_H
#define _UL_FIFO_H

/* FIFO commands */
#define UL_STATS	"ul_stats"
#define UL_RM		"ul_rm"
#define UL_RM_CONTACT   "ul_rm_contact"
#define UL_DUMP         "ul_dump"
#define UL_FLUSH        "ul_flush"
#define UL_ADD          "ul_add"
#define UL_SHOW_CONTACT "ul_show_contact"

/* buffer dimensions */
#define MAX_TABLE 128
#define MAX_USER 256

int init_ul_fifo(void);

#endif
