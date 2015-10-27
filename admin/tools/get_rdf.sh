#!/bin/tcsh

cd /usr1/public/documents/net/awf/inc/data/rdf/

# News
wget www.cnn.com/cnn.rss -O cnn.rss
wget www.heise.de/newsticker/heise.rdf -O heise.rdf
wget www.telepolis.de/news.rdf -O telepolis.rdf
wget www.hotwired.com/webmonkey/meta/headlines.rdf -O hotwired.rdf
wget headlines.internet.com/internetnews/prod-news/news.rss -O internet_dot_com.rss
wget www.kuro5hin.org/backend.rdf -O kuro5hin.rdf  
wget slashdot.org/slashdot.rdf -O slashdot.rdf  
wget www.theregister.co.uk/tonys/slashdot.rdf -O theregister.rdf
wget www.wired.com/news_drop/netcenter/netcenter.rdf -O wired.rdf

# Software
wget freshmeat.net/backend/fm.rdf -O freshmeat.rdf
