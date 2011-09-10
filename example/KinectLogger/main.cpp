/*
   Copyright (C) 2011:
         Daniel Roggen, droggen@gmail.com

   All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <QtGui/QApplication>
#include <QMessageBox>
#include <QFileInfo>
#include "mainwindow.h"

#include "cmdline/cmdline.h"

void Syntax(QString progname,QString errmsg="");

void Syntax(QString progname,QString errmsg)
{

   QString str = QString("<html><pre>\
\n\n\
%1\
\n\n\
Usage:\n\n\
      %2  [-?|-h]  [-n &lt;numuser&gt;]  [-s &lt;port&gt;]  [&lt;file&gt;]\n\n\
   ?|h        display this help\n\
   &lt;numuser&gt;  maximum number of users to store/serve (\"NaN\" are used for padding if less users are effectively available)\n\
   &lt;port&gt;     starts a streaming server on &lt;port&gt;\n\
   &lt;file&gt;     store data in &lt;file&gt;\n\
\n\
If neither a port nor a file is specified, the program displays the Kinect data without logging.</pre></html>\n").arg(errmsg).arg(progname);

   QMessageBox::information(0,progname,str);

}

int main(int argc, char *argv[])
{
   unsigned numtrack;
   unsigned help1,help2;
   char s_num[256],s_file[256],s_port[256];
   int num,port;

   QFileInfo info(argv[0]);
   QString progname = info.baseName();



   QApplication a(argc, argv);

   // Scan command line
   int rv = ScanCommandLine("-n@ -s@ @ -h -?",argc,argv,s_num,s_port,s_file,&help1,&help2);

   // Check for error or help request
   if(rv || help1 || help2)
   {
      Syntax(progname,"Program syntax");
      return 0;
   }

   // Check optional number
   if(strlen(s_num))
   {
      // Convert
      rv = sscanf(s_num,"%d",&num);
      if(rv!=1 || num<1)
      {
         Syntax(progname,"Invalid number of users");
         return 0;
      }
   }
   else
      num=1;

   // Check server port number
   if(strlen(s_port))
   {
      // Convert
      rv = sscanf(s_port,"%d",&port);
      if(rv!=1 || port<1)
      {
         Syntax(progname,"Invalid port number");
         return 0;
      }
   }
   else
      port=0;


   MainWindow w(progname,QString(s_file),num,port);
   w.show();
   rv = a.exec();

   return rv;
}
