// $Id: Gopal.java,v 1.1 2002/05/21 19:44:45 andy Exp $
package com.tagish.auth.test;

import com.tagish.auth.win32.*;
import java.util.*;

public class Gopal
{
    private Random rng;

    public Gopal()
    {
        rng = new Random();
    }

    private boolean checkDomainLogin(String loginId, String password, String domainName) throws Exception
    {
        boolean loginStatus = true;

        try  {
            NTSystem ntSystem = new NTSystem();
            ntSystem.logon(loginId, password.toCharArray(), domainName);
            System.out.println("User validated using domain login");
        } catch(Exception ex) {
            System.err.println("Exception serLoginBean:checkDomainLogin): " + ex.getMessage());
            loginStatus = false;
        }

        return loginStatus;
    }

    private String randomName(int len)
    {
        StringBuffer out = new StringBuffer(len);
        while (len-- > 0) {
            out.append((char) ('a' + rng.nextInt(26)));
        }
        return out.toString();
    }

    public static void main(String[] args)
    {
        Gopal me = new Gopal();

        try {
            for (int i = 0; i < 5; i++) {
                String login  = me.randomName(10);
                String pass   = me.randomName(10);
                String domain = "TAG";
                System.out.println(login + ", " + pass + ", " + domain);
                me.checkDomainLogin(login, pass, domain);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
