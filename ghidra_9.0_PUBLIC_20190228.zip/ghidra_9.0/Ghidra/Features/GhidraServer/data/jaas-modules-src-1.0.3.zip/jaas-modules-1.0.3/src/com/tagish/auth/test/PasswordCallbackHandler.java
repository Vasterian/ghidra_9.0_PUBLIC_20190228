// $Id: PasswordCallbackHandler.java,v 1.2 2002/05/21 19:46:40 andy Exp $
package com.tagish.auth.test;

import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;

import java.io.IOException;

/**
 * A simple call back handler to supply the username / password to JAAS.
 */
public class PasswordCallbackHandler implements CallbackHandler
{
	private String username;
	private char[] password;

	public PasswordCallbackHandler(String username, String password)
	{
		this.username = username;
		this.password = password.toCharArray();;
	}

	/**
	 * Standard impl of the CallbackHandler interface
	 */
	public void handle(Callback[] callbacks) throws	IOException, UnsupportedCallbackException
	{
		for (int i = 0; i < callbacks.length; i++)
			if (callbacks[i] instanceof NameCallback)
				((NameCallback)callbacks[i]).setName(username);
			else if (callbacks[i] instanceof PasswordCallback)
				((PasswordCallback)callbacks[i]).setPassword(password);
			else
				throw new UnsupportedCallbackException(callbacks[i], "Unrecognized Callback - only supports username/password");
   }
}
