
= Gemライブラリ


この章ではGemについて説明します。前に出てきたGemfileやbundleコマンドについて説明をしていきます。


== Gemライブラリ


プログラミングの世界では便利なプログラム集がたくさん公開されています。いろんなプログラムにて共通で使える便利な公開されたプログラムをライブラリと呼びます。RailsもWebアプリを簡単に作るライブラリの集合体です。



Rubyの世界にもライブラリがあり、Gemと呼ばれます。Gemは @<href>{https://rubygems.org,rubygems.org} で公開されていて、10万を越えるGemが登録されています。


== Gemをインストールして利用する


Gemをインストールするにはgem installコマンドを利用します。試しにawesome_printというGemをインストールしてみましょう。


//emlist[][bash]{
$ gem i awesome_print
//}

//cmd{
Fetching: awesome_print-1.7.0.gem (100%)
Successfully installed awesome_print-1.7.0
1 gem installed
//}


gem iのiはinstallの省略形です。省略せずにgem installと打っても実行することができます。コマンドを実行すると、上記のような表示が出てGemを使えるようになります。Gem名の後ろにハイフンで続く数字はバージョン名です。



インストールしたawesome_printは、きれいに表示するapというメソッドを提供します。irbを起動して試しに使ってみましょう。irbは対話形式でRubyプログラムを実行できる仕組みです。rails consoleもirbを利用しています。


//emlist[][bash]{
$ irb
//}

//emlist[][ruby]{
require "awesome_print"
ap [1,2,3]
//}

//cmd{
$ irb
> require "awesome_print"
=> true
> ap [1,2,3]
[
    [0] 1,
    [1] 2,
    [2] 3
]
=> nil
//}


画面に色付きで配列[1,2,3]が表示されれば成功です。Rubyが標準で持っているpメソッドも同等のことができますが、apメソッドの方がきれいに読み易い形で表示してくれます。



プログラムで最初に実行した@<tt>{require "awesome_print"}はapを使えるようにするためのコードです。Gemライブラリは、gem installして、require文を実行することで利用できます。


== BundlerとGemfile


Gemは前述のように、gem installコマンドで簡単にインストールすることが可能ですが、この方法でたくさんのGemをインストールしようとすると、Gemの数だけコマンドを打たなくてはいけません。それは大変なので、Gemを簡単に管理するBundlerという仕組みが用意されています。



BundlerでGem群をインストールするには2つの手順を踏みます。



1つ目はGemfileという名前のファイルに使用するGemを書くこと。2つ目はbundle installコマンドを実行することです。仕組みの名前はBundlerですが、コマンド名はbundleと最後にrが付かない点に注意です。



Railsアプリは最初からBundlerの仕組みを利用するようになっています。GemfileはRailsアプリの場合、Railsのルートフォルダに置いてあります。エディタで開いてみましょう。


//emlist[][ruby]{
source 'https://rubygems.org'


# Bundle edge Rails instead: gem 'rails', github: 'rails/rails'
gem 'rails', '~> 5.0.0'
# Use sqlite3 as the database for Active Record
gem 'sqlite3'
...
//}


Railsアプリは最初に作った状態ですでに10個以上のgemが登録されています。ここに、さきほどのawesome_print gemを追加してみましょう。書く場所はどこでも良いのですが、@<tt>{gem 'rails'}の次の行に書いてみます。


//emlist[][diff]{
gem 'rails', '~> 5.0.0'
+ gem 'awesome_print'
//}


書いて保存したら、以下のようにbundle installコマンドを実行します。ここで@<tt>{bundle install}と書くことも、また、installを省略して@<tt>{bundle}と書くこともできます。


//cmd{
$ bundle
Using rake 11.2.2
...
Using awesome_print 1.7.0
...
Bundle complete! 16 Gemfile dependencies, 64 gems now installed.
Use `bundle show [gemname]` to see where a bundled gem is installed.
//}


これでawesome_print gemを使う準備は完了です。試しにrails consoleを起動して、apメソッドを使ってみましょう。


//cmd{
$ rails c
> ap [1,2,3]
[
    [0] 1,
    [1] 2,
    [2] 3
]
=> nil
> ap Book.first
  Book Load (0.2ms)  SELECT  "books".* FROM "books" ORDER BY "books"."id" ASC LIMIT ?  [["LIMIT", 1]]
#<Book:0x007fc943e0bf78> {
            :id => 1,
         :title => "四月は君の嘘",
          :memo => "ピアニストの少年とバイオリニストの少女による音楽と青春の話",
    :created_at => Sat, 02 Jul 2016 02:44:08 UTC +00:00,
    :updated_at => Sat, 02 Jul 2016 02:44:08 UTC +00:00
}
=> nil
//}


ターミナルに改行されて色がついて読み易く表示されているかと思います。Railsアプリの場合はrequire文を書かなくても良いようになっているので、require文を書く必要がありません（ちなみに、書いても問題ありません。requireを複数実行すると、最初の1回だけが適切に実行されます）。



また、Gemfileを変更してbundleコマンドを実行すると、Gemfile.lockが更新されます。Gemfile.lockは、簡単に言うと使用されたGem名とバージョン情報が記録されています。gitなどでソース管理をしている場合は、両方のファイルを追加して管理してください。



たとえ話で説明すると、Gemfileはgemをインストールするための発注書です。bundle installコマンドを実行すると、発注書に従ってgemがインストールされます。Gemfile.lockは納品書です。発注書に基づいて実際にインストールされたgemのバージョン情報などが書かれています。


== Gemfileに書かれたGemのバージョンアップ


各Gemは随時新しいバージョンがリリースされることがあります。Gemfileに書かれたGemの新しいバージョンをインストールしたい場合は@<tt>{bundle update} コマンドを使います。実行すると、新しいバージョンのGemがある場合、バージョンアップしてGemfile.lockファイルを自動で更新します。@<tt>{bundle update}コマンドを実行すると、Gemfile中のすべてのGemがバージョンアップ対象になり、新しいバージョンがあるGemはバージョンアップされます。



特定のGemだけをバージョンアップしたい場合は、@<tt>{bundle update Gem名} とGem名を添えて実行すればOKです（この場合、指定したGemだけがバージョンアップされるのではなく、指定したGemが利用している他のGemがある場合は、セットでバージョンアップされます）。


== Gemfileでのバージョン指定


利用するバージョンを指定したい場合は、Gemfileにバージョンを追記します。RailsアプリのGemfileを見ると、いくつかバージョンがすでに指定してあるものがあります。


//emlist[][ruby]{
gem 'uglifier', '>= 1.3.0'
//}


これは「uglifierというGemのバージョンは、1.3.0以上」という意味の指定になります。これは数式の意味と同じですね。一方、こんな見慣れないバージョン指定表記があります。


//emlist[][ruby]{
gem 'coffee-rails', '~> 4.2'
//}


 ~> という記号、このケースでは、@<tt>{>= 4.2.0} かつ @<tt>{< 4.3} という意味になります。小さいバージョンアップは受け入れて、大きなバージョンアップは受け入れない、という記号です。



バージョンの表記の仕様は @<href>{http://bundler.io/v1.11/gemfile.html,Bundlerのページ} に解説されています。


== Gemの探し方


ある用途のGemで定番のものを探したい場合は、 @<href>{https://www.ruby-toolbox.com/,"The Ruby Toolbox"} というサイトが便利です。カテゴリーごとに、その用途のGemがランキング表示されています。



たとえばグラフを描くGemを探す場合はGraphicsカテゴリの @<href>{https://www.ruby-toolbox.com/categories/graphing,Graphing} というページがあります。ここでグラフを描画するためのGemがランキング形式で紹介されています。説明文から用途にあったものを探したり、ランキングが上位のものをいくつか調べて利用すると良いでしょう。開発が活発かどうかを示すDevelopment activityやLast commit欄も有用な情報です。開発が止まると、RubyやRailsのバージョンアップに追随できなくなるリスクがあるためです。


== まとめ


ポイントをまとめます。

 * いろんなプログラムにて共通で使える便利な公開されたプログラムをライブラリと呼ぶ
 * GemはRubyの世界のライブラリ
 * Gemをインストールするにはgem iコマンド（gem installの省略形）を使う
 * Bundlerは複数のGemを簡単に管理する仕組み
 * BundlerではGemfileという名前のファイルに使用するGemを書く
 * Gemfileを作成しbundleコマンド（bundle installの省略形）を実行することでGem群をインストールする
 * bundleコマンドを実行するとGemfileに加えてGemfile.lockファイルが生成されるので、両方をソース管理対象にする
