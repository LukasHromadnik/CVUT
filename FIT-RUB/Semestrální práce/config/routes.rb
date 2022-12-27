Rails.application.routes.draw do
  root 'welcome#index'

  resources :channels
  resources :articles

  get 'channels/:id/articles', to: 'channels#articles', as: 'channel_articles'
end
