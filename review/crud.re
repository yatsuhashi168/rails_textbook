
= CRUDの基礎とindexアクション


Webアプリには基本となる4つの機能があります。ページの新規作成（Create）、表示（Read）、更新（Update）、削除（Destroy）です。それぞれの頭文字を取ってCRUDと呼ばれています。大きなWebアプリも、このCRUDを基礎として作られます。



ここではCRUD機能を持ったRailsアプリを作り、その動作について解説します。


== CRUD基礎

=== アプリの作成


今回も最初にアプリを作ります。本のタイトルとメモを管理する簡易なアプリです。以前の章で作成した @<tt>{my_web_apps} フォルダの下に新しいアプリを作ります。ターミナルを起動して以下のコマンドを打ちます。


//emlist[][bash]{
cd my_web_apps
rails new books_app
//}

//cmd{
$ rails new books_app
      create
... （略）
Bundle complete! 15 Gemfile dependencies, 63 gems now installed.
Use `bundle show [gemname]` to see where a bundled gem is installed.
         run  bundle exec spring binstub --all
* bin/rake: spring inserted
* bin/rails: spring inserted
//}


次に、以下のコマンドを実行します@<fn>{1}。


//emlist[][bash]{
cd books_app
rails g scaffold book title:string memo:text
rails db:migrate
rails s
//}

//cmd{
$ rails g scaffold book title:string memo:text
Running via Spring preloader in process 52142
      invoke  active_record
      create    db/migrate/20160702024137_create_books.rb
...
      invoke  scss
      create    app/assets/stylesheets/scaffolds.scss
$ rails db:migrate
== 20160702024137 CreateBooks: migrating ======================================
-- create_table(:books)
   -> 0.0013s
== 20160702024137 CreateBooks: migrated (0.0014s) =============================

$ rails s
=> Booting Puma
=> Rails 5.0.0 application starting in development on http://localhost:3000
=> Run `rails server -h` for more startup options
Puma starting in single mode...
* Version 3.4.0 (ruby 2.3.1-p112), codename: Owl Bowl Brawl
* Min threads: 5, max threads: 5
* Environment: development
* Listening on tcp://localhost:3000
Use Ctrl-C to stop
//}


ブラウザを起動して以下のURLを入力してアクセスしてみます。

 * http://localhost:3000/books


//image[index_page][起動した画面]{
//}




こんな画面が表示されましたでしょうか。ここから、New Bookリンクをクリックして先へ進み、title欄とmemo欄を記入しCreate Bookボタンを押してみてください。



//image[created_page][データを入力してCreate Bookボタン]{
//}




ここからBackボタンを押すと最初の@<tt>{/books}のページに戻りますが、先ほど入力したデータが表示されていることが分かるかと思います。



//image[index_with_data_page][入力したデータが表示される]{
//}




さらにデータを追加したり、他のリンクも操作してみてください。



ここで作成したアプリはCRUDの各機能、すなわち、新規作成、表示、更新、削除を持っています。以降で、1つずつその動作を説明していきます。


=== scaffoldコマンド


ここまで何回か出てきたrails g scaffoldコマンドは新規作成、表示、更新、削除、各機能を一度に作る機能です。つまり、scaffoldを使うとCRUD機能を持ったページを作成することができます。



scaffoldは英語で「（建築現場などの）足場」という意味です。工事中の建物のまわりに組まれた足場の上で大工さんが作業している、あの足場です。scaffoldコマンドはWebアプリを作り易くするための足場です。CRUD機能はWebアプリでよく使う機能なので、「CRUDのこの部分にこの機能を足したらできるな」という場合に、scaffoldで生成したファイル群を編集して自分のアプリを作っていくと効率良く作れます。



作成されるファイルは以下の通りです。


//cmd{
$ rails g scaffold book title:string memo:text
  create    app/models/book.rb
  invoke    test_unit
  create      test/models/book_test.rb
  create      test/fixtures/books.yml
  invoke  resource_route
   route    resources :books
  invoke  scaffold_controller
  create    app/controllers/books_controller.rb
  invoke    erb
  create      app/views/books
  create      app/views/books/index.html.erb
  create      app/views/books/edit.html.erb
  create      app/views/books/show.html.erb
  create      app/views/books/new.html.erb
  create      app/views/books/_form.html.erb
  invoke    test_unit
  create      test/controllers/books_controller_test.rb
  invoke    helper
  create      app/helpers/books_helper.rb
  invoke      test_unit
  invoke    jbuilder
  create      app/views/books/index.json.jbuilder
  create      app/views/books/show.json.jbuilder
  invoke  assets
  invoke    coffee
  create      app/assets/javascripts/books.coffee
  invoke    scss
  create      app/assets/stylesheets/books.scss
//}


この中のroute（@<tt>{invoke resource_route}）, controller（@<tt>{invoke scaffold_controller}）やview（@<tt>{invoke erb}）は前の章で説明したrailsの各役割を担うファイルです。



一方、ページの観点から見ると、scaffoldは次に示す新規作成、表示、更新、削除のための4つの画面と、画面のない3つの機能を生成します。



//image[crud_4_pages][CRUD 4つの画面]{
//}




それぞれの機能と対応するアクションを示したものが次の図です（アクションとはコントローラに書いてあるpublicなメソッドのことです）。基本的には、1つの画面を表示するとき、1つのアクションが実行されています。アクションについてはまた後で説明します。



//image[crud_7_actions][CRUD 7つのアクション]{
//}




この中から最初に、indexアクションのページについて見ていきましょう。


== indexアクション


最初にindexアクションについて見ていきましょう。さきほどの7つのアクションの図で示されたこの部分です。



//image[index_7_actions][indexアクション]{
//}




indexアクションで表示されるページ（下の図）は登録されているデータの一覧です。ここでは登録されているBookの一覧表示画面になります。この画面が表示されるまでの処理を追いかけてみましょう。



//image[index_page_with_data][index - 一覧画面]{
//}




前にも出てきた「Railsアプリがリクエストを受けてレスポンスを返すまで」をindexアクションの場合で描きなおした図が以下になります。



//image[index_flow][Railsアプリがリクエストを受けてレスポンスを返すまで（indexアクション）]{
//}




順番にRoutesから処理を追っていきます。


=== Routes


Routesはリクエストと処理を行うコントローラを決めるための対応表です。



//image[index_flow_routes][Routes]{
//}




Routesでリクエストから次に処理するコントローラとアクションが決まります。routes対応表を表示するために、rails serverを起動して、@<tt>{http://localhost:3000/rails/info/routes} へアクセスしてみましょう。



//image[index_routes][Routes対応表]{
//}




今回はURLが"/books"、HTTPメソッドが "GET" なので、下線部が該当行です。「/booksにGETでアクセスされたとき、BooksControllerのindexアクションへ処理を流す」という意味になります。次に進むべきコントローラはbooks#index（BooksControllerのindexアクション）となることが分かります。



コントローラの処理を見る前に、Routesのコードを見てみましょう。Routesのコードは @<tt>{config/routes.rb} にあります。コードは以下の図に示すようになっています。


//emlist[][ruby]{
Rails.application.routes.draw do
  resources :books
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
//}


Routes対応表を作るコードは @<tt>{Rails.application.routes.draw do} から @<tt>{end} の間に書きます。#記号ではじまる行は分かりやすさのためのコメントで、動作には影響がありません。 @<tt>{http://localhost:3000/rails/info/routes}のページで表示された8行の対応表は、@<tt>{resources :books} の1行によって作られます。CRUDは基本的な動作でよく利用されるため、簡単に書けるようになっているのです。



それでは次の処理、コントローラへ進みましょう。


=== コントローラ


Routesではリクエストから処理されるコントローラとアクションが決まると、次はコントローラへ処理が移ります。今回はBooksControllerのindexアクションです。



//image[index_flow_contoller][コントローラ]{
//}




コントローラのコードを見てみましょう。ファイルは @<tt>{app/controllers/books_controller.rb} です。


//emlist[][ruby]{
class BooksController < ApplicationController
  ...
  def index
    @books = Book.all
  end
  ...
//}


@<tt>{class BooksController < ApplicationController}から最後の行の@<tt>{end}までがBooksControllerのコードです。この中の@<tt>{dev index}から次の@<tt>{end}までがindexアクションになります（対応するendで字下げ（インデント）が揃えて書いてあります）。



ここでやっていることは1行、@<tt>{@books = Book.all} です。Book.allでBookの全データが詰まった配列を取得して、@booksインスタンス変数へ代入します（Book.allでBookの全データがとれる仕組みについては、またあとで説明します）。



@booksは@から始まる変数、インスタンス変数です。「一番小さなRailsアプリつくり」のところでも説明したように、インスタンス変数@booksを使っているのは、次に処理を行うviewで利用するためです。もしも、ここで変数をローカル変数booksにしてしまうと、コントローラの処理が終わったところでなくなってしまいます。



また、@bookと単数形ではなく@booksと複数形になっているのはBook.allで取得するデータが複数の本のデータを格納した配列であることを示すためです。場合によっては、本のデータが1つしかない、あるいは0個である、という状況も考えられます。その場合も1個、あるいは0個のデータを格納した配列が取得されます。どのパターンも@booksは配列を指すため、変数名を複数形の@booksにするのが慣習です。細かいことのようですが、RailsやRubyでのプログラミングでは変数の複数形と単数形を意識して命名することは大切なことです。



コントローラのアクションでの処理が終わると、次はビューに処理が移ります。特に指定がない場合、@<tt>{app/views/コントローラ名/アクション名}のviewファイルへ処理が移ります。ここでは@<tt>{app/views/books/index.html.erb}です。


=== ビュー


最後はビューでレスポンスで返されるHTMLがつくられます。



//image[index_flow_view][ビュー]{
//}




ビューのコードを見てみましょう。今回は @<tt>{app/views/books/index.html.erb} です。



//image[index_view_codes][ビューのコード]{
//}




コントローラで取得した@booksを表示させる箇所を見てみましょう。表示は@<tt>{<% @books.each do |book| %>}から@<tt>{<% end %>} で行っています。@booksに代入された全データ（"ドラえもん"、"君に届け"）でブロックを繰り返し実行し、titleやmemoを表示したり、show（詳細画面）やedit（編集画面）、削除ボタンのリンクを生成します。



図に示した状態では、@booksに2つのデータが入っています。@books.each文は全データで繰り返し処理を行う文です。1回目の実行では変数book（2つの@<tt>{|}記号で囲まれている所）にドラえもんのデータが入った状態でtitleやmemoが表示されます。2回目の実行では君に届けのデータが格納されて同様の処理が実行されます。



index.html.erbはHTMLのテンプレートファイルで、HTMLの中にRubyコードを埋め込むことができます。@<tt>{<%= %>}で囲まれたコードは評価結果がHTMLとして出力されます。@<tt>{<% %>} で囲まれたコードは結果が出力されずに実行されます（今回は@books.eachの繰り返しで @<tt>{<% %>} が使われています）。



RailsアプリはつくられたHTMLをレスポンスとしてブラウザに返します。ブラウザは受け取ったHTMLを私たちが見れる形で表示します。


== まとめ


この章ではCRUDの1つ、一覧表示（index画面）の基礎的な内容について説明しました。



リクエストを受けてレスポンスを返すまでの流れをまとめると以下のようになります。Railsアプリはリクエストを受けると、 Routes、コントローラ、ビューの処理を経てレスポンス（HTML）を返します。



//image[index_flow_2][Railsアプリがリクエストを受けてレスポンスを返すまで（indexアクション）]{
//}




今回のリクエストはindex（一覧）画面を表示するものです。上記の処理の後、ブラウザは受け取ったHTMLを私たちが見れる形で表示します。



//image[index_page_with_data_2][index画面]{
//}




ポイントをまとめると以下のようになります。

 * CRUDはWebアプリの基本となる4つの機能、新規作成（Create）、表示（Read）、更新（Update）、削除（Destroy）
 * rails g scaffoldはCRUDを生成する機能
 * パスが/books、HTTPメソッドがGETのリクエストはRoutes、コントローラ、ビューの処理を経て一覧画面を表示する


次の章ではCRUDのCreate（新規作成）について説明します。


== さらに学びたい場合は
 * @<href>{http://railsguides.jp/action_controller_overview.html,Rails Guides : Action Controller の概要}
 ** コントローラについての詳しい解説です。
 * @<href>{http://railsguides.jp/routing.html,Rails Guides : Rails のルーティング}
 ** routesについての詳しい解説です。

//footnote[1][Rails4.2以前では @<tt>{rails db:migrate} の替わりに @<tt>{bin/rake db:migrate} と実行してください。]
