// $Id: Password.java,v 1.4 2002/05/21 19:46:40 andy Exp $
package com.tagish.auth.tools;

import com.tagish.auth.Utils;

/**
 * Simple command line tool to encode passwords in a form suitable
 * for use by DBLogin and FileLogin based login schemes.
 */
public class Password
{
	public static void main(String[] args)
	{
		try
		{
			for (int a = 0; a < args.length; a++)
				System.out.println(args[a] + ": " + Utils.cryptPassword(args[a].toCharArray()));
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}
}