import React from 'react'
import { View, Text, Image, Button } from 'react-native'
import ImagePicker from 'react-native-image-picker'

export default class App extends React.Component {

  // Component state
  state = {
    photo: null,
  }


  // Function for photo gallery
  handleChoosePhoto = () => {
    const options = {
      noData: true,
    }
    ImagePicker.launchImageLibrary(options, response => {
      if (response.uri) {
        this.setState({ photo: response })
      }
    })
  }


  // Function for http post 
  handleUploadPhoto = () => {
    fetch("http://ecf0da31.ngrok.io/upload", {
      method: "POST",
      body: this.state.photo.uri
    })
      .then(response => response.text())
      .then(response => {
        console.log("upload succes", response);
        alert("Upload success!");
        this.setState({ photo: null });
      })
      .catch(error => {
        console.error(error);
        console.log("upload error", error);
        alert("Upload failed!");
      });
  };

  // GUI render
  render() {
    const { photo } = this.state
    return (
      <View style={{ flex: 1, alignItems: 'center', justifyContent: 'center' }}>
        {photo && (
          <React.Fragment>
            <Image
              source={{ uri: photo.uri }}
              style={{ width: 300, height: 300 }}
            />
            <Button title="Upload" onPress={this.handleUploadPhoto} />
          </React.Fragment>
        )}
        <Button title="Choose Photo" onPress={this.handleChoosePhoto} />
      </View>
    )
  }
}


// FormData create function
const createFormData = (photo) => {
  const data = new FormData();

  data.append("photo", photo.uri);

  return data;
}
