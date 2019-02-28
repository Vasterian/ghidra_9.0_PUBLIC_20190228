// $Id: Login.java,v 1.4 2002/05/21 19:46:40 andy Exp $
package com.tagish.auth.test;

/*
 * Rudely hacked from Sun's original Jaas example by Andy. The original
 * contained these copyright notices, so here they are:
 *
 * Copyright 2000 Sun Microsystems, Inc. All rights reserved.
 * Copyright 2000 Sun Microsystems, Inc. Tous droits reserves.
 */
import java.io.*;
import java.util.*;
import java.security.Principal;
import javax.security.auth.*;
import javax.security.auth.callback.*;
import javax.security.auth.login.*;
import javax.security.auth.spi.*;
import com.tagish.auth.*;

/**
 * <p> This Sample application attempts to authenticate a user
 * and executes a SampleAction as that user.
 *
 * <p> If the user successfully authenticates itself,
 * the username and number of Credentials is displayed.
 *
 * @version 1.19, 01/11/00
 */
public class Login
{
	/**
	 * Attempt to authenticate the user.
	 *
	 * <p>
	 *
	 * @param args input arguments for this application.  These are ignored.
	 */
	public static void main(String[] args)
	{
		// use the configured LoginModules for the "Login" entry
		LoginContext lc = null;
		try
		{
			lc = new LoginContext("NTLogin", new LoginCallbackHandler());
		}
		catch (LoginException le)
		{
			le.printStackTrace();
			System.exit(-1);
		}

		// the user has 3 attempts to authenticate successfully
		int i;
		for (i = 0; i < 3; i++)
		{
			try
			{
				// attempt authentication
				lc.login();

				// if we return with no exception, authentication succeeded
				break;
			}
			catch (AccountExpiredException aee)
			{
				System.out.println("Your account has expired.  " +
				"Please notify your administrator.");
				System.exit(-1);
			}
			catch (CredentialExpiredException cee)
			{
				System.out.println("Your credentials have expired.");
				System.exit(-1);
			}
			catch (FailedLoginException fle)
			{
				System.out.println("Authentication Failed");
				try { Thread.currentThread().sleep(3000); } catch (Exception e) { }
			}
			catch (Exception e)
			{
				System.out.println("Unexpected Exception - unable to continue");
				e.printStackTrace();
				System.exit(-1);
			}
		}

		// did they fail three times?
		if (i == 3)
		{
			System.out.println("Sorry");
			System.exit(-1);
		}

		// let's see what Principals we have
		Iterator principalIterator = lc.getSubject().getPrincipals().iterator();
		System.out.println("Authenticated user has the following Principals:");
		while (principalIterator.hasNext())
		{
			Principal p = (Principal) principalIterator.next();
			System.out.println("\t" + p.toString());
		}

		System.out.println("User has " + lc.getSubject().getPublicCredentials().size() +
							" Public Credential(s)");

		// now try to execute the SampleAction as the authenticated Subject
		//Subject.doAs(lc.getSubject(), new SampleAction());

		System.exit(0);
	}
}
