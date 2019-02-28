// $Id: LoginStressTest.java,v 1.2 2002/05/21 19:46:40 andy Exp $
package com.tagish.auth.test;

import javax.security.auth.login.LoginContext;

public class LoginStressTest
{
	public LoginStressTest()
	{
		System.getProperties().setProperty("java.security.auth.login.config",
			this.getClass().getClassLoader().getResource("tagish.login").toString());
	}

	public void run() throws Exception
	{
		for(int i=0; i<=50; i++) {
			new Thread(new LoginThread(i)).start();
			Thread.sleep(1000);
		}
	}

	public static class LoginThread implements Runnable
	{
		int id = 0;

		public LoginThread(int id)
		{
			this.id = id;
		}

		public void run()
		{
			try {
				LoginContext lc = new LoginContext("NTLogin", new PasswordCallbackHandler("vpulipati", "yshirtL32"));
				lc.login();
				System.out.println("** thread " + id + "  logged in");
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public static void main(String[] args)
	{
		try {
			new LoginStressTest().run();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
